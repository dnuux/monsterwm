# Maintained by dnuux <dnuuxx@gmail.com>
pkgname=monsterwm
pkgver=20120902
pkgrel=1
pkgdesc="Very minimal, lightweight, monstrous, tiny, dynamic window manager"
url="https://github.com/c00kiemon5ter/monsterwm"
arch=('i686' 'x86_64')
license=('MIT')
groups=('modified')
depends=('libxinerama')
conflicts=('monsterwm-git' 'monsterwm-xcb')
source=("Makefile"
        "config.h"
        "monsterwm.c"
        "monsterwm.1"
        "LICENSE")

build() {
  cd "${srcdir}"
  make
}

package() {
  cd "${srcdir}"
  make PREFIX=/usr DESTDIR="${pkgdir}" install
  install -Dm644 "LICENSE" "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"
}
