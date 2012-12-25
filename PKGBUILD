# Maintainer: dnuux <dnuuxx@gmail.com>
pkgname=monsterwm
pkgver=20121225
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
md5sums=('f7d6353d7882fa69bde77e4ac65382a0'
         '896ede709a6fc771728d7742d67f6fc0'
         '5f9fa7ee768c3a19b33c3f083347346d'
         '4ec00c94b3783092e25b4ca7f6c540ea'
         'c97bdb63d9ef7d6ec9b67d9b02649cc8')

build() {
  cd "${srcdir}"
  make
}

package() {
  cd "${srcdir}"
  make PREFIX=/usr DESTDIR="${pkgdir}" install
  install -Dm644 "LICENSE" "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"
}
