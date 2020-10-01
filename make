 nix-shell -p gcc premake5 gnumake --pure --run "premake5 gmake && make $1"
