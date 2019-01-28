#include "erl_nif.h"
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <stdio.h>

#define MAXBUFLEN 1024

static ERL_NIF_TERM
raise_exception(ErlNifEnv *env, const char* message);

static ERL_NIF_TERM
ffmpeg_get_file_info(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  AVFormatContext *av_context;
  AVStream *av_stream;
  ERL_NIF_TERM fileinfo;
  ERL_NIF_TERM key;
  ERL_NIF_TERM val;
  int duration;

  char path[MAXBUFLEN];
  (void)memset(&path, '\0', sizeof(path));
  enif_get_string(env, argv[0], path, sizeof(path), ERL_NIF_LATIN1);

  av_context = avformat_alloc_context();
  if(!av_context) {
    return raise_exception(env, "error in avformat_alloc_context()");
  }

  if(avformat_open_input(&av_context, path, NULL, NULL) != 0) {
    return raise_exception(env, "error in avformat_open_input()");
  }

  if(avformat_find_stream_info(av_context, NULL) < 0) {
    return raise_exception(env, "error in avformat_find_stream_info()");
  }

  fileinfo = enif_make_new_map(env);

  /* format */
  key = enif_make_string(env, "format", ERL_NIF_LATIN1);
  val = enif_make_string(env, av_context->iformat->long_name, ERL_NIF_LATIN1);
  enif_make_map_put(env, fileinfo, key, val, &fileinfo);

  ERL_NIF_TERM streams[av_context->nb_streams];

  int i;
  for(i = 0; i < av_context->nb_streams; i++) {
    av_stream = av_context->streams[i];
    ERL_NIF_TERM stream = enif_make_new_map(env);

    /* type */
    key = enif_make_string(env, "type", ERL_NIF_LATIN1);
    val = enif_make_string(env, av_get_media_type_string(av_stream->codecpar->codec_type), ERL_NIF_LATIN1);
    enif_make_map_put(env, stream, key, val, &stream);

    /* duration */
    duration = av_stream->duration * av_q2d(av_stream->time_base);
    key = enif_make_string(env, "duration", ERL_NIF_LATIN1);
    val = enif_make_int(env, duration);
    enif_make_map_put(env, stream, key, val, &stream);

    streams[i] = stream;
  }

  key = enif_make_string(env, "streams", ERL_NIF_LATIN1);
  val = enif_make_list_from_array(env, streams, av_context->nb_streams);
  enif_make_map_put(env, fileinfo, key, val, &fileinfo);

  avformat_close_input(&av_context);
  avformat_free_context(av_context);

  return fileinfo;
}

static ERL_NIF_TERM
raise_exception(ErlNifEnv *env, const char* message) {
  ERL_NIF_TERM reason = enif_make_string(env, message, ERL_NIF_LATIN1);
  return enif_raise_exception(env, reason);
}

static ErlNifFunc nif_funcs[] = {
  {"get_file_info", 1, ffmpeg_get_file_info}
};

ERL_NIF_INIT(Elixir.FFbindings, nif_funcs, NULL, NULL, NULL, NULL);
