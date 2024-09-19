class Besign < Formula
  desc "Program for make signature object on pdf"  
  homepage "https://github.com/karyakampak/homebrew-besign"
  url "https://github.com/karyakampak/homebrew-besign/archive/refs/tags/v1.0.0.tar.gz"
  sha256 "093038b3e1191d73d5fdf5a85363f774ee714e07a16a6680af88f6e69fca3667"
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
    python3 = Formula["python@3"].opt_bin/"python3"
    pip = Formula["python@3"].opt_bin/"pip3"
    venv_path = buildpath/"venv"

    # Create and activate a virtual environment
    system "#{python3}", "-m", "venv", venv_path
    system "#{venv_path}/bin/pip", "install", "setuptools", "Cython", "numpy", "pymupdf", "qrcode[pil]", "pillow"

    # Ensure pymupdf (fitz) is correctly installed
    system "#{venv_path}/bin/python3", "-c", "import fitz"

    # Set environment variables to use the virtual environment
    site_packages = "#{venv_path}/lib/python#{Formula["python@3"].version.major_minor}/site-packages"

    cd "build" do
      ENV["PYTHONPATH"] = site_packages
      ENV.prepend_path "PATH", "#{venv_path}/bin"

      system "cmake", "..",  "-DPYTHON_EXECUTABLE=#{venv_path}/bin/python3", *std_cmake_args
      system "make"
      system "make", "install"
    end

    # Ensure PYTHONPATH is set correctly when besign runs
    bin.env_script_all_files(libexec/"bin", :PYTHONPATH => site_packages)

  end

  test do
    system "#{bin}/besign", "--version"
  end
end