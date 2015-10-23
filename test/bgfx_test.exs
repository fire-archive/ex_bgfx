defmodule BgfxTest do
  use ExUnit.Case
  doctest Bgfx

  test "the truth" do
    assert 1 + 1 == 2
  end

  #test "bgfx null renderer test" do
  #  assert Bgfx.Nif.bgfx_init(:null, 0, 0, :undefined, :undefined) == :ok
  #end

  #test "bgfx d3d12 renderer test" do
  #  assert Bgfx.Nif.bgfx_init(:direct3d12, 0, 0, :undefined, :undefined) == :ok
  #end

  test "bgfx d3d12 renderer test extended" do
    #assert Bgfx.Nif.bgfx_init(:direct3d12, 0, 0, :undefined, :undefined) == :ok
    width = 1270
    height = 768
    #assert Bgfx.Nif.reset(width, height, 0x00000080) == :ok
    use Bitwise
    #assert Bgfx.Nif.set_view_clear(0, bor(0x0001, 0x0002), 0x303030ff, 1.0, 0) #== :ok
  end

  #test "bgfx opengl renderer test extended" do
  #  assert Bgfx.Nif.bgfx_init(:opengl, 0, 0, :undefined, :undefined) == :ok
  #  assert Bgfx.Nif.reset(1280, 768, 0x00000080) == :ok
  #end
end
