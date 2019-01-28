defmodule FFbindings do
  @on_load :load_nifs

  def load_nifs do
    :erlang.load_nif('./ffbindings', 0)
  end

  def file_info(path) when is_binary(path) do
    path
    |> String.to_charlist()
    |> get_file_info()
  end

  def file_info(path) when is_list(path) do
    path
    |> get_file_info()
  end

  def file_info(path), do: raise "invalid type for path: #{inspect(path)}"

  defp get_file_info(_path) do
    raise "NIF get_file_info/1 not implemented"
  end
end
