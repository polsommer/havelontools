rem
rem $Header: oci18.sql 14-jul-99.14:15:26 mjaeger Exp $
rem
rem Copyright (c) 1995, 1999, Oracle Corporation.  All rights reserved.
rem
rem    NAME
rem      oci18.sql
rem    DESCRIPTION
rem      Script for A22400 OCI Techniques White Paper
rem      Demo script for oci18.c
rem    MODIFIED   (MM/DD/YY)
rem     mjaeger    07/14/99 -  bug 808870: OCCS: convert tabs, no long lines
rem     cchau      08/18/97 -  enable dictionary protection
rem     echen      01/10/97 -  change internal to sys/change_on_install
rem     vraghuna   03/01/95 -  Creation

set echo on;
connect sys/knl_test7 as sysdba;
drop user ocitest cascade;
create user ocitest identified by ocitest;
grant connect,resource to ocitest;
connect ocitest/ocitest;

create table oci18tab (col1 varchar2(30));

create or replace package oci18pkg as

        type char_array is table of varchar2(30) index by binary_integer;

        function oci18func(
                col1      in char_array,   -- array to put cname in
                numins    in integer)
        return integer;

end;
/

create or replace package body oci18pkg as


        function oci18func(
                col1      in char_array,
                numins    in integer
        ) return integer is retval integer;

        begin

                for i in 1..numins loop
                        insert into oci18tab values (col1(i));
                end loop;
                commit;

                retval := numins;
                return(retval);

        end;

end;
/

commit;

