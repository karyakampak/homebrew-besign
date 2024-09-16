class Myprogram < Formula
  desc "My awesome program"
  homepage "https://github.com/karyakampak/corebesign"
  url "https://github.com/karyakampak/corebesign/archive/refs/tags/v1.0.0.tar.gz"
  sha256 "<sha256_of_tarball>"
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