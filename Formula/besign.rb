class Besign < Formula
  desc "My awesome program"
  homepage "https://github.com/karyakampak/homebrew-besign"
  url "https://github.com/karyakampak/homebrew-besign/archive/refs/tags/v1.0.1.tar.gz"
  sha256 "b8c12e3611e6da0ae4d317da31cd55147ae744f294bb1aa50df210ee981afd44"
  license "MIT"

  depends_on "cmake" => :build
  depends_on "python"
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

