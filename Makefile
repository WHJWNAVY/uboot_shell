PKG_NAME:=ushell

CP:=cp -fpR
TOPDIR:=$(shell pwd)

INSTALL_BIN:=install -m0755
INSTALL_DIR:=install -d -m0755

PKG_EXE_NAME:=ushell

PKG_SOURCE_DIR:=$(TOPDIR)/src
PKG_INSTALL_DIR:=$(TOPDIR)/install
PKG_BUILD_DIR:=$(TOPDIR)/build

#CFLAGS +=
LDFLAGS += -lpthread -lm

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	#$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/install
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_EXE_NAME) $(PKG_INSTALL_DIR)
endef

define Package/build
	cd $(PKG_BUILD_DIR) && cmake $(PKG_SOURCE_DIR) && make && cd -
endef

build:
	$(Build/Prepare)
	$(Package/build)

install:
	$(Package/install)

all: build

clean:
	rm -rf $(PKG_BUILD_DIR)

.PHONY: all clean build install
