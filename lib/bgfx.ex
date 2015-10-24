defmodule Bgfx do
  use Application

  def start(_type, _args) do
    #Supervisor.start_link [], strategy: :one_for_one
    run
  end

  def run do
    width = 1280
    height = 768
    window = Bgfx.Nif.sdl_create_window
    Bgfx.Nif.bgfx_init(:direct3d12, 0, 0, :undefined, :undefined, window)
    Bgfx.Nif.reset(width, height, 0x00000080)
    #use Bitwise
    #assert Bgfx.Nif.set_view_clear(0, bor(0x0001, 0x0002), 0x303030ff, 1.0, 0) == :ok
    Bgfx.Nif.run(window)
    :ok
  end    
end
