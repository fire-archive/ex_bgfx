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

  def bgfx_init do
	  _bgfx_init
  end

  def _hello do
    exit(:nif_library_not_loaded)
  end
  
  def _bgfx_init do
    exit(:nif_library_not_loaded)
  end
end
