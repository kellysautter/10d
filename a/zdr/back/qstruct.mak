#-------------------------------------------------------------------
# use NMAKE for this make file
# DESCRIPTION  Construct .HST (Entity structure) files from .LOD
#-------------------------------------------------------------------

!INCLUDE \$(KZV)\MAK\KZVERS.HNM
!INCLUDE \$(KZV)\MAK\KZCOMMON.HNM
kz_srca = $(kz_basea)\zdr
kz_inc  = $(kz_basea)\inc

ALL : $(kz_inc)z\tzwdlgso.hst \
      $(kz_inc)r\kzwdlgxo.hst \

clean :
    -del $(kz_inc)z\tzwdlgso.hst
    -del $(kz_inc)r\kzwdlgxo.hst

$(kz_inc)r\tzwdlgso.hst:      $(kz_bin)\sys\tzwdlgso.xod
   qspawn $(@B).err @<<
qstruct $(kz_bin)\sys\tzwdlgso.xod -O$(kz_inc)z\tzwdlgso.hst
<<

$(kz_inc)r\kzwdlgxo.hst:      $(kz_bin)\sys\kzwdlgxo.xod
   qspawn $(@B).err @<<
qstruct $(kz_bin)\sys\kzwdlgxo.xod -O$(kz_inc)r\kzwdlgxo.hst
<<
