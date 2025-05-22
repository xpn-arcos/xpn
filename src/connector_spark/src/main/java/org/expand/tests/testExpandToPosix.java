package org.expand.tests;

import java.nio.ByteBuffer;

import org.expand.jni.ExpandToPosix;
import org.expand.jni.Stat;
import org.expand.jni.ExpandFlags;

public class testExpandToPosix {

	public static final String ANSI_RESET = "\u001B[0m";
	public static final String ANSI_GREEN = "\u001B[32m";
	public static final String ANSI_RED = "\u001B[31m";
	public static final String ANSI_YELLOW = "\u001B[33m";
	public static final String ANSI_BLUE = "\u001B[34m";

	public static void main(String[] args) {

		ExpandToPosix xpn = new ExpandToPosix();
		byte b[] = new byte[65536];
		byte b_read[] = new byte[65536];
		byte c = 'a';
		ByteBuffer buf = ByteBuffer.allocateDirect(b.length);
		ByteBuffer buf_read = ByteBuffer.allocateDirect(b_read.length);
		for (int i = 0; i < b.length; i++) {
			buf.put(i, c);
		}

		try {
			xpn.jni_xpn_init();
			System.out.println(ANSI_BLUE + "COMIENZO LAS PRUEBAS" + ANSI_RESET);

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_CREAT" + ANSI_RESET);
			int fd1 = xpn.jni_xpn_creat("/xpn/testing", xpn.flags.S_IRWXO | xpn.flags.S_IRWXG | xpn.flags.S_IRWXU);
			if (fd1 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Descriptor de fichero de creacion correcto: " + fd1);
			int fd2 = xpn.jni_xpn_creat("/xpn/test/testing", xpn.flags.S_IRWXO | xpn.flags.S_IRWXG | xpn.flags.S_IRWXU);
			if (fd2 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Descriptor de fichero en directorio no existente: " + fd2);

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_EXIST" + ANSI_RESET);
			int i1 = xpn.jni_xpn_exist("/xpn/testing");
			if (i1 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Resultado de fichero existente: " + i1);
			int i2 = xpn.jni_xpn_exist("/xpn/testing2");
			if (i2 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Resultado de fichero no existente: " + i2);

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_CLOSE" + ANSI_RESET);
			int i3 = xpn.jni_xpn_close(fd2);
			if (i3 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Resultado de fichero mal cerrado: " + i3);
			int i4 = xpn.jni_xpn_close(fd1);
			if (i4 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Resultado de fichero bien cerrado: " + i4);

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_OPEN" + ANSI_RESET);
			int fd3 = xpn.jni_xpn_open("/xpn/test/testing", xpn.flags.O_RDWR);
			if (fd3 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Descriptor de fichero abierto no existente: " + fd3);
			int fd4 = xpn.jni_xpn_open("/xpn/testing", xpn.flags.O_RDWR);
			if (fd4 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Descriptor de fichero abierto: " + fd4);

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_WRITE" + ANSI_RESET);
			long i5 = xpn.jni_xpn_write(fd3, buf, b.length);
			if (i5 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Escritura en fd malo: " + i5);
			long i6 = xpn.jni_xpn_write(fd4, buf, b.length);
			if (i6 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Escritura correcta: " + i6);

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_READ" + ANSI_RESET);
			long i7 = xpn.jni_xpn_read(fd3, buf_read, b_read.length);
			if (i7 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Lectura en fd malo: " + i7);
			long i8 = xpn.jni_xpn_read(fd4, buf_read, b_read.length);
			if (i8 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Lectura correcta: " + i8);
			xpn.jni_xpn_close(fd4);

			// for (int a = 0; a < b_read.length; a++) {
			// 	System.out.println(buf_read.get(a));
			// }

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_RENAME" + ANSI_RESET);
			int i9 = xpn.jni_xpn_rename("/xpn/kk", "/xpn/testing3");
			if (i9 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Rename de fichero no existente: " + i9);
			int i10 = xpn.jni_xpn_rename("/xpn/testing", "/xpn/testing4");
			if (i10 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Rename correcto: " + i10);

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_MKDIR" + ANSI_RESET);
			int i11 = xpn.jni_xpn_mkdir("/xpn/kk/test", (short) 0777);
			if (i11 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Creacion de directorio en uno no existente: " + i11);
			int i12 = xpn.jni_xpn_mkdir("/xpn/test", (short) 0777);
			if (i12 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Creacion de directorio correcto: " + i12);
			int i13 = xpn.jni_xpn_mkdir("/xpn/test", (short) 0777);
			if (i13 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Creacion de directorio ya existente: " + i13);

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_GETDIRCONTENT" + ANSI_RESET);
			String [] str1 = xpn.jni_xpn_getDirContent("/xpn");
			if (str1.length == 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
				System.out.println("Tamano de directorio existente: " + str1.length);
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
				System.out.println("Tamano de directorio existente: " + str1.length);
				for (int z = 0; z < str1.length; z++) {
					System.out.println("Entrada: " + str1[z]);
				}
			}
			String [] str2 = xpn.jni_xpn_getDirContent("/xpn/kk");
			if (str2.length == 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Tamano de directorio no existente: " + str2.length);

			System.out.println(ANSI_YELLOW + "PRUEBAS DE XPN_UNLINK y XPN_RMDIR" + ANSI_RESET);
			xpn.jni_xpn_creat("/xpn/test/testing", xpn.flags.S_IRWXO | xpn.flags.S_IRWXG | xpn.flags.S_IRWXU);
			int i17 = xpn.jni_xpn_rmdir("/xpn/kk");
			if (i17 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Borrado de directorio no existente: " + i17);
			int i18 = xpn.jni_xpn_rmdir("/xpn/test");
			if (i18 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Borrado de directorio con fichero: " + i18);
			int i19 = xpn.jni_xpn_unlink("/xpn/peo");
			if (i19 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			}
			System.out.println("Unlink de fichero no existente: " + i19);
			int i20 = xpn.jni_xpn_unlink("/xpn/test/testing");
			if (i20 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Unlink correcto: " + i20);
			int i21 = xpn.jni_xpn_rmdir("/xpn/test");
			if (i21 < 0) {
				System.out.print(ANSI_RESET + "[" + ANSI_RED + "ERROR" + ANSI_RESET + "] ");
			} else {
				System.out.print(ANSI_RESET + "[" + ANSI_GREEN + "OK" + ANSI_RESET + "] ");
			}
			System.out.println("Borrado de directorio correcto: " + i21);
			xpn.jni_xpn_unlink("/xpn/testing4");

			System.out.println(ANSI_BLUE + "FIN DE LAS PRUEBAS" + ANSI_RESET);

			xpn.jni_xpn_destroy();
		}catch (Exception e){
			System.out.println("EXCEPCION: " + e);
		}
	}
}
