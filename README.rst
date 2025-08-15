LAVFilters connector for Kirikiri Z
===================================

This plugin allows the usage of
`LAVFilters <https://github.com/Nevcairiel/LAVFilters>`__ in `KirikiriZ
/ 吉里吉里Z <https://github.com/krkrz/krkrz>`__.

Building
--------

After placing ``ncbind`` and ``tp_stub`` in the parent directory, and
setting up the MinGW-w64 environment, a simple ``make`` will generate
``krdslavf.dll``.

How to use
----------

After ``Plugins.link("krdslavf.dll");`` is used when ``LAVAudio.ax``,
``LAVSplitter.ax``, and ``LAVVideo.ax`` from
`LAVFilters <https://github.com/Nevcairiel/LAVFilters>`__ is registered,
the ``VideoOverlay`` class will play formats supported by LAVFilters
when ``VideoOverlay.mode`` is not set to ``vomMFEVR``.

License
-------

This project is licensed under the GNU General Public License GPL
version 2 or later license. Please read the ``COPYING`` file for more
information.
