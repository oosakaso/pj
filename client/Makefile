include $(TOPDIR)/rules.mk

PKG_NAME:=getinfo
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk


define Package/getinfo
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=getinfo
	DEPENDS:=+libc
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/getinfo/install
	$(INSTALL_DIR) $(1)/bin
	$(CP) /lib/x86_64-linux-gnu/libc.so.6 $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/getinfo $(1)/bin/
endef

$(eval $(call BuildPackage,getinfo))
