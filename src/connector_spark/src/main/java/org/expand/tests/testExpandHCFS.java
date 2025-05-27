package org.expand.tests;

import java.net.URI;
import java.nio.ByteBuffer;
import java.io.IOException;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.BlockLocation;
import org.apache.hadoop.fs.permission.FsPermission;

import org.expand.hadoop.Expand;
import org.expand.hadoop.ExpandOutputStream;

public class testExpandHCFS {

	public static final String ANSI_RESET = "\u001B[0m";
	public static final String ANSI_GREEN = "\u001B[32m";
	public static final String ANSI_RED = "\u001B[31m";
	public static final String ANSI_YELLOW = "\u001B[33m";
	public static final String ANSI_BLUE = "\u001B[34m";

    public static void main(String[] args) {
        Expand xpn = new Expand();
        URI uri = URI.create("xpn:///");
        Configuration conf = new Configuration();
        conf.set("fs.defaultFS", "xpn:///");
        conf.set("fs.xpn.impl", "Expand");
        FSDataOutputStream out_str;
        FSDataInputStream in_str;
        FileStatus fst;
        FileStatus fst_arr[];
        BlockLocation blckloc[];
        byte b[] = new byte[65536];
        byte b_read[] = new byte [65536];
        byte c = 'a';
		for (int i = 0; i < b.length; i++) {
			b[i] = c;
		}

        try {
            xpn.initialize(uri, conf);

            System.out.println(ANSI_BLUE + "COMIENZO LAS PRUEBAS" + ANSI_RESET);
            System.out.println(ANSI_YELLOW + "PRUEBAS DE CREATE" + ANSI_RESET);
            xpn.create(new Path("/xpn/test"));
            try {
                xpn.getFileStatus(new Path("/xpn/test"));
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Creacion de fichero vacio");
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Creacion de fichero vacio");
            }

            try {
                xpn.create(new Path("/xpn/test"), FsPermission.getFileDefault(), false, 8192, (short) 1, (long) 8192, null);
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Creacion de fichero duplicado sin overwrite");
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Creacion de fichero duplicado sin overwrite");
            }

            try {
                xpn.create(new Path("/xpn/fold_test/test"));
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Creacion de fichero en directorio no existente");
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Creacion de fichero en directorio no existente");
            }

            try {
                out_str = xpn.create(new Path("/xpn/test"));
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Creacion de fichero duplicado con overwrite");

                out_str.write(b, 0, b.length);
                out_str.close();
                fst = xpn.getFileStatus(new Path("/xpn/test"));
                if (fst.getLen() == b.length) {
                    System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Escritura en fichero");
                } else {
                    System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Escritura en fichero");
                }
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Creacion de fichero duplicado con overwrite");
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE EXISTS" + ANSI_RESET);
            boolean bool = xpn.exists(new Path("/xpn/test"));
            if (bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Fichero existente");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Fichero existente");
            }
            bool = xpn.exists(new Path("/xpn/test_no"));
            if (!bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Fichero no existente");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Fichero no existente");
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE GET FILE STATUS" + ANSI_RESET);
            try {
                fst = xpn.getFileStatus(new Path("/xpn/test_no"));
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Get file status de fichero no existente");
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Get file status de fichero no existente");
            }
            try {
                fst = xpn.getFileStatus(new Path("/xpn/test"));
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Get file status de fichero existente");
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Get file status de fichero existente");
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE GET FILE BLOCK LOCATIONS" + ANSI_RESET);
            fst = xpn.getFileStatus(new Path("/xpn/test"));
            blckloc = xpn.getFileBlockLocations(fst, 0, fst.getLen() - 1);
            for (int i = 0; i < blckloc.length; i++){
                System.out.println("BLOCK LOC: " + blckloc[i].toString());
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE OPEN" + ANSI_RESET);
            try {
                xpn.open(new Path("/xpn/test_no"));
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Apertura de fichero no existente");
            } catch (NullPointerException npe) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Apertura de fichero no existente");
            }
            in_str = xpn.open(new Path("/xpn/test"));
            if (in_str == null) {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Apertura de fichero existente");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Apertura de fichero existente");
            }
            int readed = in_str.read(b_read, 0, b_read.length);
            in_str.close();
            if (readed == b.length) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Lectura de fichero");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Lectura de fichero");
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE APPEND" + ANSI_RESET);
            out_str = xpn.create(new Path("/xpn/fold_test/test_app"));
            out_str.write(b, 0, b.length);
            out_str.close();
            fst = xpn.getFileStatus(new Path("/xpn/fold_test/test_app"));
            if (fst.getLen() == b.length) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Append y escritura en fichero no existente");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Append y escritura en fichero no existente");
            }

            out_str = xpn.append(new Path("/xpn/test"));
            out_str.write(b, 0, b.length);
            out_str.close();
            fst = xpn.getFileStatus(new Path("/xpn/test"));
            if (fst.getLen() == b.length * 2) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Append y escritura en fichero existente");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Append y escritura en fichero existente");
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE MKDIRS" + ANSI_RESET);
            bool = xpn.mkdirs(new Path("/xpn/test_fold_1"), FsPermission.getFileDefault());
            if (bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Crear directorio");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Crear directorio");
            }
            bool = xpn.mkdirs(new Path("/xpn/test_fold_1/0/1/2/3/4"), FsPermission.getFileDefault());
            if (bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Crear directorio recursivo");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Crear directorio recursivo");
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE IS DIRECTORY" + ANSI_RESET);
            bool = xpn.isDirectory(new Path("/xpn/test_no"));
            if (!bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] isDir de ruta no existente");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] isDir de ruta no existente");
            }
            bool = xpn.isDirectory(new Path("/xpn/test"));
            if (!bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] isDir de fichero");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] isDir de fichero");
            }
            bool = xpn.isDirectory(new Path("/xpn/fold_test"));
            if (bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] isDir de directorio");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] isDir de directorio");
            }


            System.out.println(ANSI_YELLOW + "PRUEBAS DE LIST STATUS" + ANSI_RESET);
            try {
                fst_arr = xpn.listStatus(new Path("/xpn/test_no"));
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] List status de fichero no existente");
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] List status de fichero no existente");
            }
            try {
                fst_arr = xpn.listStatus(new Path("/xpn/test"));
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] List status de fichero");
                for(int i = 0; i < fst_arr.length; i++) {
                    System.out.println(fst_arr[i].toString());
                }
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] List status de fichero");
            }
            try {
                fst_arr = xpn.listStatus(new Path("/xpn"));
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] List status de directorio");
                for(int i = 0; i < fst_arr.length; i++) {
                    System.out.println(fst_arr[i].toString());
                }
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] List status de directorio");
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE RENAME" + ANSI_RESET);
            bool = xpn.rename(new Path("/xpn/test_no"), new Path("/xpn/test_no2"));
            if (!bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Rename de fichero no existente");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Rename de fichero no existente");
            }
            bool = xpn.rename(new Path("/xpn/test"), new Path("/xpn/fold_test/test_app"));
            if (!bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Rename a fichero existente");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Rename a fichero existente");
            }
            bool = xpn.rename(new Path("/xpn/test"), new Path("/xpn/test_new"));
            if (bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Rename correcto");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Rename correcto");
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE GET LENGTH" + ANSI_RESET);
            try {
                xpn.getLength(new Path("/xpn/test_nonono"));
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Get length de directorio no existente");
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Get length de directorio no existente");
            }
            try {
                if (xpn.getLength(new Path("/xpn/fold_test")) == xpn.getFileStatus(new Path("/xpn/fold_test")).getLen()){
                    System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Get length de directorio existente");
                } else {
                    System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Get length de directorio existente");
                }
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Get length de directorio existente");
            }
            try {
                if (xpn.getLength(new Path("/xpn/test_new")) == xpn.getFileStatus(new Path("/xpn/test_new")).getLen()){
                    System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Get length de fichero existente");
                } else {
                    System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Get length de fichero existente");
                }
            } catch (IOException ioe) {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Get length de fichero existente");
            }

            System.out.println(ANSI_YELLOW + "PRUEBAS DE DELETE" + ANSI_RESET);
            bool = xpn.delete(new Path("/xpn/test_no"), false);
            if (!bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Delete fichero no existente");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Delete fichero no existente");
            }
            bool = xpn.delete(new Path("/xpn/fold_test"), false);
            if (!bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Delete directorio lleno sin recursividad");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Delete directorio lleno sin recursividad");
            }
            bool = xpn.delete(new Path("/xpn/test_fold_1/0/1/2/3/4"), false);
            if (bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Delete directorio vacio sin recursividad");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Delete directorio vacio sin recursividad");
            }
            bool = xpn.delete(new Path("/xpn/test_fold_1"), true);
            if (bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Delete fichero lleno con recursividad");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Delete fichero lleno con recursividad");
            }
            bool = xpn.delete(new Path("/xpn/test_new"), false);
            if (bool) {
                System.out.println(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] Delete fichero");
            } else {
                System.out.println(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] Delete fichero");
            }
            xpn.delete(new Path("/xpn/fold_test"), true);

            System.out.println(ANSI_BLUE + "FIN DE LAS PRUEBAS" + ANSI_RESET);
            

        } catch (Exception e){
			System.out.println("EXCEPCION: " + e);
		}
    }
}
