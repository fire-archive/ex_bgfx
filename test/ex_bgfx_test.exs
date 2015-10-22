defmodule ExBgfxTest do
  use ExUnit.Case
  doctest ExBgfx

  test "the truth" do
    assert 1 + 1 == 2
  end

  test "hello" do
    assert ExBgfx.Nif.hello == 0.0
  end

  test "bgfx null renderer test" do
    assert ExBgfx.Nif.bgfx_init(:opengl, 0, 0, :undefined, :undefined) == true
  end

  #test "bgfx d3d12 renderer test" do
  #  assert ExBgfx.Nif.bgfx_init(3,0,0,:undefined,:undefined) == true
  #end
end
