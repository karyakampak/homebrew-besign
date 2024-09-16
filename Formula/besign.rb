class Besign < Formula
  desc "Program for make signature object on pdf"
  homepage "https://github.com/karyakampak/homebrew-besign"
  url "https://github.com/karyakampak/homebrew-besign/archive/refs/tags/v1.0.2.tar.gz"
  sha256 "17db8fd706e9ee656721b7aff54b79936b2fb6cc4b96434b2d26e01621d4987a"
  license "MIT"

  depends_on "cmake" => :build
  # depends_on "python@3"
  # depends_on "opencv"
  depends_on "qrencode"
  depends_on "openssl"
  depends_on "curl"
  depends_on "nlohmann-json"
  depends_on "ossp-uuid"


  def install
    cd "build" do
      system "cmake", "..", *std_cmake_args
      system "make"
      system "make", "install"
    end
  end

  test do
    system "#{bin}/dksign", "--version"
  end
end

