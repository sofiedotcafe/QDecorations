{
  config,
  lib,
  pkgs,
  ...
}:

with lib;

let
  cfg = config.qt.decoration;

  decorationPackages = {
    catppuccin = pkgs.qcatppuccindecorations;
  };

  makeQtPluginPaths =
    package: versions: args:
    map (
      version:
      let
        useQt6Flag = version == "qt6";
        qt = if useQt6Flag then pkgs.qt6 else pkgs.qt5;

        overriddenPackage =
          let
            attempt = builtins.tryEval (package.override ({ useQt6 = useQt6Flag; } // args));
          in
          if attempt.success then
            attempt.value
          else
            (trace "Warning: package ${toString package} does not accept 'useQt6'. Using package as-is." package);
      in
      "${overriddenPackage}/${qt.qtbase.qtPluginPrefix}"
    ) versions;
in

{
  options.qt.decoration = {
    name = mkOption {
      type = types.nullOr (
        types.addCheck types.str (
          name:
          cfg ? "package"
          ||
            name
              # is in the list of known decorations
              # if package is not set
              # otherwise allow anything
              elem
              name
              (builtins.attrNames decorationPackages)
        )
      );
      description = "Name of the Qt decoration theme";
    };

    package = mkOption {
      type = types.package;
      description = "Package providing the Qt decoration plugin";
    };

    args = mkOption {
      type = types.attrsOf types.any;
      default = { };
      description = "Additional arguments for the decoration package";
    };
  };

  config = mkIf (cfg.name != null) (
    let
      decorationPackage = cfg.package or decorationPackages.${cfg.name};

      qtPluginPaths = makeQtPluginPaths decorationPackage [ "qt5" "qt6" ] cfg.args;
    in
    {
      qt.enable = true;

      home.sessionSearchVariables = {
        QT_PLUGIN_PATH = qtPluginPaths;
      };

      home.sessionVariables = {
        QT_WAYLAND_DECORATION = cfg.name;
      };

      xsession.importedVariables = [ "QT_WAYLAND_DECORATION" ];

      systemd.user.sessionVariables = mkForce {
        QT_PLUGIN_PATH = concatStringsSep ":" config.home.sessionSearchVariables.QT_PLUGIN_PATH;
        QML2_IMPORT_PATH = concatStringsSep ":" (
          config.home.sessionSearchVariables.QML2_IMPORT_PATH or [ ]
        );
      };
    }
  );
}
