{
  description = "Simple non-posix compliant shell for learning purposes";

  inputs = {
    nixpkgs.url = "nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
  flake-utils.lib.eachDefaultSystem (system: let
      pkgs = import nixpkgs { inherit system; };
    in {
      packages.default = pkgs.stdenv.mkDerivation {
        name = "molsh";
        src = ./.;

        nativeBuildInputs = [ pkgs.clang ];

        installPhase = ''mkdir -p $out/bin; install -m0755 build/molsh $out/bin'';
      };
      devShells.default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [ clangStdenv gnumake clang clang-tools lldb gdb];
        NIX_LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
          pkgs.stdenv.cc.cc
        ];
        NIX_LD = pkgs.lib.fileContents "${pkgs.stdenv.cc}/nix-support/dynamic-linker";
      };
    }
  );
}
