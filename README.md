
[![Build Status](https://travis-ci.org/hrbrmstr/msgxtractr.svg?branch=master)](https://travis-ci.org/hrbrmstr/msgxtractr) [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/hrbrmstr/msgxtractr?branch=master&svg=true)](https://ci.appveyor.com/project/hrbrmstr/msgxtractr) [![codecov](https://codecov.io/gh/hrbrmstr/msgxtractr/branch/master/graph/badge.svg)](https://codecov.io/gh/hrbrmstr/msgxtractr)

`msgxtractr` : Read Outlook '.msg' Files

'Microsoft' 'Outlook' messages can be saved in '.msg' files. Tools are provided that enable extraction of metadata, envelope, headers, body and attachments from these files.

The following functions are implemented:

-   `read_msg`: Read in an Outlook '.msg' file

### Installation

``` r
devtools::install_github("hrbrmstr/msgxtractr")
```

### Usage

``` r
library(msgxtractr)

# current verison
packageVersion("msgxtractr")
```

    ## [1] '0.2.0'

``` r
print(str(read_msg(system.file("extdata/unicode.msg", package="msgxtractr"))))
```

    ## List of 7
    ##  $ headers         :Classes 'tbl_df', 'tbl' and 'data.frame':    1 obs. of  18 variables:
    ##   ..$ Return-path               : chr "<brizhou@gmail.com>"
    ##   ..$ Received                  :List of 1
    ##   .. ..$ : chr [1:4] "from st11p00mm-smtpin007.mac.com ([17.172.84.240])\nby ms06561.mac.com (Oracle Communications Messaging Server "| __truncated__ "from mail-vc0-f182.google.com ([209.85.220.182])\nby st11p00mm-smtpin007.mac.com\n(Oracle Communications Messag"| __truncated__ "by mail-vc0-f182.google.com with SMTP id ie18so3484487vcb.13 for\n<brianzhou@me.com>; Mon, 18 Nov 2013 00:26:25 -0800 (PST)" "by 10.58.207.196 with HTTP; Mon, 18 Nov 2013 00:26:24 -0800 (PST)"
    ##   ..$ Original-recipient        : chr "rfc822;brianzhou@me.com"
    ##   ..$ Received-SPF              : chr "pass (st11p00mm-smtpin006.mac.com: domain of brizhou@gmail.com\ndesignates 209.85.220.182 as permitted sender)\"| __truncated__
    ##   ..$ DKIM-Signature            : chr "v=1; a=rsa-sha256; c=relaxed/relaxed;        d=gmail.com;\ns=20120113; h=mime-version:date:message-id:subject:f"| __truncated__
    ##   ..$ MIME-version              : chr "1.0"
    ##   ..$ X-Received                : chr "by 10.221.47.193 with SMTP id ut1mr14470624vcb.8.1384763184960;\nMon, 18 Nov 2013 00:26:24 -0800 (PST)"
    ##   ..$ Date                      : chr "Mon, 18 Nov 2013 10:26:24 +0200"
    ##   ..$ Message-id                : chr "<CADtJ4eNjQSkGcBtVteCiTF+YFG89+AcHxK3QZ=-Mt48xygkvdQ@mail.gmail.com>"
    ##   ..$ Subject                   : chr "Test for TIF files"
    ##   ..$ From                      : chr "Brian Zhou <brizhou@gmail.com>"
    ##   ..$ To                        : chr "brianzhou@me.com"
    ##   ..$ Cc                        : chr "Brian Zhou <brizhou@gmail.com>"
    ##   ..$ Content-type              : chr "multipart/mixed; boundary=001a113392ecbd7a5404eb6f4d6a"
    ##   ..$ Authentication-results    : chr "st11p00mm-smtpin007.mac.com; dkim=pass\nreason=\"2048-bit key\" header.d=gmail.com header.i=@gmail.com\nheader."| __truncated__
    ##   ..$ x-icloud-spam-score       : chr "33322\nf=gmail.com;e=gmail.com;pp=ham;spf=pass;dkim=pass;wl=absent;pwl=absent"
    ##   ..$ X-Proofpoint-Virus-Version: chr "vendor=fsecure\nengine=2.50.10432:5.10.8794,1.0.14,0.0.0000\ndefinitions=2013-11-18_02:2013-11-18,2013-11-17,19"| __truncated__
    ##   ..$ X-Proofpoint-Spam-Details : chr "rule=notspam policy=default score=0 spamscore=0\nsuspectscore=0 phishscore=0 bulkscore=0 adultscore=0 classifie"| __truncated__
    ##  $ sender          :List of 2
    ##   ..$ sender_email: chr "brizhou@gmail.com"
    ##   ..$ sender_name : chr "Brian Zhou"
    ##  $ recipients      :List of 2
    ##   ..$ :List of 3
    ##   .. ..$ display_name : NULL
    ##   .. ..$ address_type : chr "SMTP"
    ##   .. ..$ email_address: chr "brianzhou@me.com"
    ##   ..$ :List of 3
    ##   .. ..$ display_name : NULL
    ##   .. ..$ address_type : chr "SMTP"
    ##   .. ..$ email_address: chr "brizhou@gmail.com"
    ##  $ subject         : chr "Test for TIF files"
    ##  $ body            : chr "This is a test email to experiment with the MS Outlook MSG Extractor\r\n\r\n\r\n-- \r\n\r\n\r\nKind regards\r\n"| __truncated__
    ##  $ attachments     :List of 2
    ##   ..$ :List of 4
    ##   .. ..$ filename     : chr "importOl.tif"
    ##   .. ..$ long_filename: chr "import OleFileIO.tif"
    ##   .. ..$ mime         : chr "image/tiff"
    ##   .. ..$ content      : raw [1:969674] 49 49 2a 00 ...
    ##   ..$ :List of 4
    ##   .. ..$ filename     : chr "raisedva.tif"
    ##   .. ..$ long_filename: chr "raised value error.tif"
    ##   .. ..$ mime         : chr "image/tiff"
    ##   .. ..$ content      : raw [1:1033142] 49 49 2a 00 ...
    ##  $ display_envelope:List of 2
    ##   ..$ display_cc: chr "Brian Zhou"
    ##   ..$ display_to: chr "brianzhou@me.com"
    ## NULL

### Test Results

``` r
library(msgxtractr)
library(testthat)

date()
```

    ## [1] "Sat Aug 26 06:07:23 2017"

``` r
test_dir("tests/")
```

    ## testthat results ========================================================================================================
    ## OK: 3 SKIPPED: 0 FAILED: 0
    ## 
    ## DONE ===================================================================================================================

### Code of Conduct

Please note that this project is released with a [Contributor Code of Conduct](CONDUCT.md). By participating in this project you agree to abide by its terms.
