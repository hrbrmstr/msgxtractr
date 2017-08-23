.msg <- NULL

.onLoad <- function(libname, pkgname) {
  .msg <<- reticulate::import("ExtractMsg", delay_load = TRUE)
}