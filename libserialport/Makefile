include $(TOPDIR)/rules.mk

PKG_NAME:=libserialport
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/sigrokproject/libserialport.git
PKG_SOURCE_VERSION:=21b3dfe5f68c205be4086469335fd2fc2ce11ed2
PKG_HASH:=e30073dfe7acdb9782727812ec708beef0754f11a82fdb01ad90c252a56077d8

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/autotools.mk

define Package/libserialport
	CATEGORY:=Libraries
	TITLE:=libserialport
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(PKG_BUILD_DIR)/autogen.sh
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/.libs/libserialport.so* $(1)/usr/lib

	$(INSTALL_DATA) $(PKG_BUILD_DIR)/libserialport.h $(1)/usr/include
endef

define Package/libserialport/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/.libs/libserialport.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,libserialport))

