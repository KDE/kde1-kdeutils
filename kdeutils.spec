%define version Beta3
%define name kdeutils
Name: %{name}
Summary: K Desktop Environment - Utilities
Version: %{version}
Release: 1
Source: ftp.kde.org:/pub/kde/unstable/CVS/snapshots/current/%{name}-%{version}.tar.gz
Group: X11/KDE/Utilities
BuildRoot: /tmp/realhot_%{name}
Copyright: GPL
Requires: qt >= 1.31
Distribution: KDE
Packager: Magnus Pfeffer <pfeffer@unix-ag.uni-kl.de>
Vendor: The KDE Team

%description
Utilities for the K Desktop Environment.

Included with this package are:

karm: personal time tracker
kcalc: scientific calculator
kedit: simple text editor
kfloppy: floppy formatting tool
khexedit: hex editor
kjots: note taker
kljettool: HP printer configuration tool
knotes: post-it notes for the desktop
kzip: archive handling tool

%prep
rm -rf $RPM_BUILD_ROOT

%setup -n kdeutils

%build
export KDEDIR=/opt/kde
./configure --prefix=$KDEDIR --with-install-root=$RPM_BUILD_ROOT
make

%install
make install

cd $RPM_BUILD_ROOT
find . -type d | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' > $RPM_BUILD_DIR/file.list.%{name}
find . -type f | sed 's,^\.,\%attr(-\,root\,root) ,' >> $RPM_BUILD_DIR/file.list.%{name}
find . -type l | sed 's,^\.,\%attr(-\,root\,root) ,' >> $RPM_BUILD_DIR/file.list.%{name}

%clean
rm -rf $RPM_BUILD_ROOT

%files -f ../file.list.%{name}
