include $(TOPDIR)/rules.mk

PKG_NAME:=tuya-esp-controller
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/tuya-esp-controller
	CATEGORY:=Base system
	TITLE:=Tuya ESP Controller
	DEPENDS:=+libtuya-sdk +esp-controller +libubus +libubox +libblobmsg-json
endef

define Package/tuya-esp-controller/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuya-esp-controllerd $(1)/usr/bin
	$(INSTALL_BIN) ./files/tuya-esp-controller.init $(1)/etc/init.d/tuya-esp-controllerd
	$(INSTALL_BIN) ./files/tuya-esp-controller.config $(1)/etc/config/tuya-esp-controllerd
endef

$(eval $(call BuildPackage,tuya-esp-controller))
