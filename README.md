# LAVFilters connector for KirikiriZ

This plugin allows the usage of LAVFilters in KirikiriZ / 吉里吉里Z.

## Building

After placing `ncbind` and `tp_stub` in the parent directory, a simple `make` 
will generate `krdslavf.dll`.

## How to use

After `Plugins.link("krdslavf.dll");` is used when LAVFilters contents is 
extracted in `$(exepath)/plugin/LAVFilters/` so that `LAVAudio.ax`, 
`LAVSplitter.ax`, and `LAVVideo.ax` are located in the aformentioned folder, 
the `VideoOverlay` class will play formats supported by LAVFilters when 
`VideoOverlay.mode` is not set to `vomMFEVR`.

## License

This project is licensed under the GNU General Public License GPL version 2 or
later license. Please read the `COPYING` file for more information.
