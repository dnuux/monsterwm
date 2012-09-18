# Maintainer: dnuux <dnuuxx@gmail.com>
pkgname=monsterwm
pkgver=20120918
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
         'd133db4259e73bcce943822d5d6558b3'
         'ad058aa4bfae907e1dbe6f4ee9aed4e2'
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
