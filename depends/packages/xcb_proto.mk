package=xcb_proto
GCCFLAGS?=
$(package)_version=1.10
$(package)_download_path=https://xcb.freedesktop.org/dist
$(package)_file_name=xcb-proto-$($(package)_version).tar.bz2
$(package)_sha256_hash=7ef40ddd855b750bc597d2a435da21e55e502a0fefa85b274f2c922800baaf05

define $(package)_set_vars
  $(package)_config_opts +=--libdir=$($($(package)_type)_prefix)/lib
  $(package)_cxxflags_aarch64_linux = $(GCCFLAGS)
  $(package)_cflags_aarch64_linux = $(GCCFLAGS)
  $(package)_cxxflags_arm_linux = $(GCCFLAGS)
  $(package)_cflags_arm_linux = $(GCCFLAGS)
endef

define $(package)_config_cmds
  $($(package)_autoconf)
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef

define $(package)_postprocess_cmds
  find -name "*.pyc" -delete && \
  find -name "*.pyo" -delete
endef
