# ffbindings

## compile C

```sh
cc -fPIC -I/usr/local/lib/erlang/erts-10.2.3/include -lavformat -dynamiclib -undefined dynamic_lookup -o ffbindings.so ffbindings.c
```

## compile Elixir

```elixir
$> iex
iex> c "ffbindings.ex"
[FFbindings]
```

## run Elixir

```elixir
iex> FFbindings.file_info("sample.mp4")
%{
  'format' => 'QuickTime / MOV',
  'streams' => [
    %{'duration' => 10, 'type' => 'video'},
    %{'duration' => 10, 'type' => 'audio'},
    %{'duration' => 10, 'type' => 'audio'},
    %{'duration' => 10, 'type' => 'data'}
  ]
}
```

## link

[dnlserrano.dev](https://dnlserrano.dev/2019/03/10/elixir-nifs.html)
