defmodule ExBgfx.Nif do
  # When your move/rename this file (and you should) make sure
  # you also change the ERL_NIF_INIT call at the bottom of
  # c_src/#{name}_nif.c
  @on_load :init

  app = Mix.Project.config[:app]
  
  def init do
    path = :filename.join(:code.priv_dir(unquote(app)), 'lib_bgfx')
    :ok  = :erlang.load_nif(path, 1)
  end

  # A simple wrapper around the NIF call
  def hello do
    _hello
  end

  def bgfx_init(type, vendor_id, device_id, callback, reallocator) do
    case type do
      :null -> 0        # No rendering.
			:direct3d9 -> 1   # Direct3D 9.0
			:direct3d11 -> 2  # Direct3D 11.0
			:direct3d12 -> 3  # Direct3D 12.0
			:metal ->  4      # Metal
			:opengles -> 5    # OpenGL ES 2.0+
			:opengl -> 6      # OpenGL 2.1+
			:vulkan -> 7      # Vulkan
			:count -> 8
      _ -> exit(type)
    end
    |> _bgfx_init(vendor_id, device_id, callback, reallocator)
    #:erlang.bump_reductions(5000)
  end

  def _hello do
    exit(:nif_library_not_loaded)
  end
  
  def _bgfx_init(_,_,_,_,_) do
    exit(:nif_library_not_loaded)
  end
end
