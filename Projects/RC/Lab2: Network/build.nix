let
  pkgs = import <nixpkgs> { };
in
pkgs.stdenv.mkDerivation {
  name = "download";
  src = ./.;
  installFlags = [ "PREFIX=$(out)" ];

  installPhase = ''
    echo $src 
    echo $out 
    # mkdir -p $out/result
    cp bin/download $out
  '';
}
