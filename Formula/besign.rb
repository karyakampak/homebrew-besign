class Besign < Formula
  desc "My awesome program"
  homepage "https://github.com/karyakampak/besign"
  url "https://github.com/karyakampak/besign/archive/refs/tags/v1.0.0.tar.gz"
  sha256 "8dcc4836dea4e249c9712e5e3eadb6108b2a97b78fb796b42dbc193030ee673b"
  license "MIT"

  depends_on "cmake" => :build

  def install
    system "cmake", ".", *std_cmake_args
    system "make"
    system "make", "install"
  end

  test do
    system "#{bin}/dksign", "--version"
  end
end
