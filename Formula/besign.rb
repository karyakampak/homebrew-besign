class Besign < Formula
  desc "Program for make signature object on pdf"  
  homepage "https://github.com/karyakampak/homebrew-besign"
  url "https://github.com/karyakampak/homebrew-besign/archive/refs/tags/v1.0.4-dev.tar.gz"
  sha256 "aa1f4ea4e6f0d1de4a4829fb1b1667d073b1bb98e4b82b82671c4668a8caa4cb"
  license "MIT"

  depends_on "cmake" => :build
  depends_on "poppler"
  depends_on "opencv"
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
    system "#{bin}/besign", "--version"
  end
end