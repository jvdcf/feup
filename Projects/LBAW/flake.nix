{
  description = ''
    A Nix-flake-based Laravel development environment
    Warning: This shell is not pure and assumes that Docker is configured!
    See this link on how to install Docker: https://medium.com/thelinux/how-to-install-the-docker-in-nixos-with-simple-steps-226a7e9ef260
    Template: https://github.com/the-nix-way/dev-templates/blob/main/php/flake.nix
  '';

  inputs.nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0.1.*.tar.gz";

  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f:
        nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import nixpkgs { inherit system; };
          }
        );
    in
    {
      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              php83
              php83Packages.composer
              docker_26
              nodejs_23
            ];
            shellHook = ''
            cd lbaw24023
            docker compose up -d
            # (steam-run ~/.local/share/JetBrains/Toolbox/apps/phpstorm/bin/phpstorm . &) > /dev/null
            php artisan config:clear
            php artisan config:cache
	          php artisan db:seed
	          '';
          };
        }
      );
    };
}
