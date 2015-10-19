defmodule ExBgfxTest do
  use ExUnit.Case
  doctest ExBgfx

  test "the truth" do
    assert 1 + 1 == 2
  end

  test "hello" do
    assert ExBgfx.Nif.hello == 0.0
  end
end
