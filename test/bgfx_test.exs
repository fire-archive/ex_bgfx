defmodule BgfxTest do
  use ExUnit.Case
  doctest Bgfx

  test "the truth" do
    assert 1 + 1 == 2
  end

  test "bgfx null renderer test" do
    assert Bgfx.Nif.bgfx_init(:null, 0, 0, :undefined, :undefined) == true
  end

  test "bgfx d3d12 renderer test" do
    assert Bgfx.Nif.bgfx_init(:direct3d12, 0, 0, :undefined, :undefined) == true
  end

  test "bgfx opengl renderer test" do
    assert Bgfx.Nif.bgfx_init(:opengl, 0, 0, :undefined, :undefined) == true
  end
end
