include $(TOPDIR)/rules.mk

PKG_NAME:=esp-controller
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/esp-controller
	CATEGORY:=Base system
	TITLE:=ESP Controller
	DEPENDS:=+libserialport +libubus +libubox +libblobmsg-json
endef

define Package/esp-controller/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/esp-controller $(1)/usr/bin
	$(INSTALL_BIN) ./files/esp-controller $(1)/etc/init.d/esp-controller

endef

$(eval $(call BuildPackage,esp-controller))