pkgname=xecute
pkgver=1.0.0
pkgrel=1
pkgdesc="even simpler alternative to sudo, doas and rdo"
arch=('any')
url=https://github.com/Jojodicus/xecute
license=('MIT')

source=("https://github.com/Jojodicus/xecute/releases/download/v${pkgver}/xecute_${pkgver}.tar.gz")
sha512sums=('10e11a472a1ce304922289e115f834e9a8d0f8c0f6e2637376d6a036733ed7741366da2abd9983a370a83a41b5933662e3647240947dd8bcb11b628b4b964d9d')

build() {
    cd $srcdir
    make
}

package() {
    cd $srcdir
    make DESTDIR="$pkgdir/" install
}
