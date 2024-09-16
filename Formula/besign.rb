class Besign < Formula
  desc "My awesome program"
  homepage "https://github.com/karyakampak/homebrew-besign"
  url "https://github.com/karyakampak/homebrew-besign/archive/refs/tags/v1.0.1.tar.gz"
  sha256 "b8c12e3611e6da0ae4d317da31cd55147ae744f294bb1aa50df210ee981afd44"
  license "MIT"

  depends_on "cmake" => :build
  depends_on "python" # Python 3 is installed via Homebrew and will be available for CMake
  depends_on "opencv"
  depends_on "qrencode"
  depends_on "openssl"
  depends_on "curl"
  depends_on "nlohmann-json"
  depends_on "ossp-uuid"

  def install
    # Define the pip configuration path
    pip_conf_dir = "#{ENV["HOME"]}/.config/pip"
    pip_conf_file = "#{pip_conf_dir}/pip.conf"

    # Create the pip configuration directory and file
    mkdir_p pip_conf_dir
    (path/pip_conf_file).write <<~EOS
      [global]
      break-system-packages = true
    EOS

    # Install Python dependencies directly
    # Ensure pip is available
    python_version = Language::Python.major_minor_version "python3"
    system "pip#{python_version}", "install", "fitz"
    system "pip#{python_version}", "install", "qrcode"
    system "pip#{python_version}", "install", "Pillow"

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

