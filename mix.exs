defmodule Mix.Tasks.Compile.Bgfx do
  @shortdoc "Compiles Bgfx"
  
  def run(_) do
    if match? {:win32, _}, :os.type do
      File.mkdir("_build")
      File.cd("_build")
      {result, _error_code} = System.cmd("cmake", ["-GVisual Studio 14 Win64", "-DCMAKE_INSTALL_PREFIX=..", ".."], stderr_to_stdout: true)
      IO.binwrite result
      {result, _error_code} = System.cmd("cmake", ["--build", ".", "--target", "install"], stderr_to_stdout: true)
      IO.binwrite result
    else
      {result, _error_code} = System.cmd("make", ["priv/bgfx.so"], stderr_to_stdout: true)
      IO.binwrite result
    end
    
    :ok
  end
end

defmodule ExBgfx.Mixfile do
  use Mix.Project
  
  def project do
    [app: :ex_bgfx,
     version: "0.0.1",
     elixir: "~> 1.1",
     build_embedded: Mix.env == :prod,
     start_permanent: Mix.env == :prod,
     deps: deps,
     compilers: [:bgfx, :elixir, :app]]
  end
  
  # Configuration for the OTP application
  #
  # Type "mix help compile.app" for more information
  def application do
    [applications: [:logger]]
  end

  # Dependencies can be Hex packages:
  #
  #   {:mydep, "~> 0.3.0"}
  #
  # Or git/path repositories:
  #
  #   {:mydep, git: "https://github.com/elixir-lang/mydep.git", tag: "0.1.0"}
  #
  # Type "mix help deps" for more examples and options
  defp deps do
    []
  end



end
