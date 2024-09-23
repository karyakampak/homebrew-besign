class Besign < Formula
  desc "Program for make signature object on pdf"  
  homepage "https://github.com/karyakampak/homebrew-besign"
  url "https://github.com/karyakampak/homebrew-besign/archive/refs/tags/v1.0.0-release.tar.gz"
  sha256 "0193d0694a274ec5fffae8531929aa7bbc520eba5639daf26eeccbb9f3034535"
  license "MIT"

  depends_on "cmake" => :build
  depends_on "python@3"
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