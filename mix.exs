defmodule ExBgfx.Mixfile do
  use Mix.Project
  
  def project do
    [app: :bgfx,
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
    [mod: {Bgfx, []}, 
    applications: [ :logger]]
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
    [{:exrm, "~> 0.19"}]
  end

end

defmodule Mix.Tasks.Compile.Bgfx do
  @shortdoc "Compiles Bgfx"
  
  def run(_) do
    if match? {:win32, _}, :os.type do
      starting_dir = System.cwd()
      case :code.priv_dir(:bgfx) do
        {:error, _error_code} -> working_dir = :filename.join(:filename.dirname(Ebin), "priv") 
          priv -> working_dir = to_string priv
          #_ -> working_dir = to_string :code.priv_dir(:bgfx)
      end  
      File.mkdir(working_dir)
      File.cd(working_dir)
      {result, _error_code} = System.cmd("cmake", ["-GVisual Studio 14 2015 Win64", "-DCMAKE_INSTALL_PREFIX=" <> working_dir, "-DCMAKE_SYSTEM_VERSION=10.0", starting_dir], stderr_to_stdout: true) 
      IO.binwrite result
      {result, _error_code} = System.cmd("cmake", ["--build", working_dir, "--target", "install"], stderr_to_stdout: true) 
      IO.binwrite result
      File.cd(starting_dir)
    else
      {result, _error_code} = System.cmd("make", ["priv/bgfx.so"], stderr_to_stdout: true)
      IO.binwrite result
    end
    
    :ok
  end
end
