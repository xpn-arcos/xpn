#include "org_expand_jni_ExpandToPosix.h"
#include "xpn.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define MAX_USR_NAME 32
#define MAX_GR_NAME 32

void parseXPNStat (JNIEnv *env, struct stat *stats, jobject *obj){
	int num_fields = 13;
    	char *field_names[] =
    	{"st_dev", "st_ino", "st_mode", "st_nlink", "st_uid", "st_gid", "st_rdev", 
		"st_size", "st_blksize", "st_blocks", "st_atime", "st_mtime", "st_ctime" };
    	
	char *field_types[] =
    	{"J", "J", "I", "I", "J", "J", "J", "J", "I", "J", "J", "J", "J" };
	
	jclass structObjClass = (*env)->FindClass(env, "org/expand/jni/Stat");
	jfieldID fids[num_fields];
	
	for (int i = 0; i < num_fields; i++) {
        	fids[i] = (*env)->GetFieldID(env, structObjClass, field_names[i], field_types[i]);
	}
	
	*obj = (*env)->AllocObject(env, structObjClass);
	(*env)->SetLongField(env, *obj, fids[0], stats->st_dev);
	(*env)->SetLongField(env, *obj, fids[1], stats->st_ino);
	(*env)->SetIntField(env, *obj, fids[2], stats->st_mode);
	(*env)->SetIntField(env, *obj, fids[3], stats->st_nlink);
	(*env)->SetLongField(env, *obj, fids[4], stats->st_uid);
	(*env)->SetLongField(env, *obj, fids[5], stats->st_gid);
	(*env)->SetLongField(env, *obj, fids[6], stats->st_rdev);
	(*env)->SetLongField(env, *obj, fids[7], stats->st_size);
	(*env)->SetIntField(env, *obj, fids[8], stats->st_blksize);
	(*env)->SetLongField(env, *obj, fids[9], stats->st_blocks);
	(*env)->SetLongField(env, *obj, fids[10], stats->st_atime);
	(*env)->SetLongField(env, *obj, fids[11], stats->st_mtime);
	(*env)->SetLongField(env, *obj, fids[12], stats->st_ctime);
}

JNIEXPORT jobject JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1createFlags
  (JNIEnv *env, jobject obj){
	
	int num_fields = 29;
	jfieldID fids[num_fields];

	char *field_names[] = {
		"S_IRWXU", "S_IRUSR", "S_IWUSR", "S_IXUSR", "S_IRWXG", "S_IRGRP", 
		"S_IWGRP", "S_IXGRP", "S_IRWXO", "S_IROTH", "S_IWOTH", "S_IXOTH",
	
		"O_WRONLY", "O_RDONLY", "O_RDWR", "O_APPEND", "O_ASYNC", "O_CLOEXEC",
        "FD_CLOEXEC", "O_CREAT", "O_DIRECTORY", "O_EXCL", "O_NOCTTY", "O_NOFOLLOW", 
		"O_NONBLOCK", "O_TRUNC",
	
		"SEEK_SET", "SEEK_CUR", "SEEK_END"};

	char *field_types[] = {
		"J", "J", "J", "J", "J", "J", 
		"J", "J", "J", "J", "J", "J",

		"J", "J", "J", "J", "J", "J", 
		"J", "J", "J", "J", "J", "J", 
		"J", "J",

		"J", "J", "J"};
	
	jclass fieldsClass = (*env)->FindClass(env, "org/expand/jni/ExpandFlags");
	for (int i = 0; i < num_fields; i++){
		fids[i] = (*env)->GetFieldID(env, fieldsClass, field_names[i], field_types[i]);
	}
	
	jobject inst = (*env)->AllocObject(env, fieldsClass);
	
	(*env)->SetLongField(env, inst, fids[0], S_IRWXU);
	(*env)->SetLongField(env, inst, fids[1], S_IRUSR);
	(*env)->SetLongField(env, inst, fids[2], S_IWUSR);
	(*env)->SetLongField(env, inst, fids[3], S_IXUSR);
	(*env)->SetLongField(env, inst, fids[4], S_IRWXG);
	(*env)->SetLongField(env, inst, fids[5], S_IRGRP);
	(*env)->SetLongField(env, inst, fids[6], S_IWGRP);
	(*env)->SetLongField(env, inst, fids[7], S_IXGRP);
	(*env)->SetLongField(env, inst, fids[8], S_IRWXO);
	(*env)->SetLongField(env, inst, fids[9], S_IROTH);
	(*env)->SetLongField(env, inst, fids[10], S_IWOTH);
	(*env)->SetLongField(env, inst, fids[11], S_IXOTH);

	(*env)->SetLongField(env, inst, fids[12], O_WRONLY);
	(*env)->SetLongField(env, inst, fids[13], O_RDONLY);
	(*env)->SetLongField(env, inst, fids[14], O_RDWR);
	(*env)->SetLongField(env, inst, fids[15], O_APPEND);
	(*env)->SetLongField(env, inst, fids[16], O_ASYNC);
	(*env)->SetLongField(env, inst, fids[17], O_CLOEXEC);
	(*env)->SetLongField(env, inst, fids[18], FD_CLOEXEC);
	(*env)->SetLongField(env, inst, fids[19], O_CREAT);
	(*env)->SetLongField(env, inst, fids[20], O_DIRECTORY);
	(*env)->SetLongField(env, inst, fids[21], O_EXCL);
	(*env)->SetLongField(env, inst, fids[22], O_NOCTTY);
	(*env)->SetLongField(env, inst, fids[23], O_NOFOLLOW);
	(*env)->SetLongField(env, inst, fids[24], O_NONBLOCK);
	(*env)->SetLongField(env, inst, fids[25], O_TRUNC);

	(*env)->SetLongField(env, inst, fids[26], SEEK_SET);
	(*env)->SetLongField(env, inst, fids[27], SEEK_CUR);
	(*env)->SetLongField(env, inst, fids[28], SEEK_END);
	
	return inst;
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1chdir
  (JNIEnv *env, jobject obj, jstring str){
	
	int str_len = (*env)->GetStringLength(env, str);
	char cstr[str_len + 1];
	(*env)->GetStringUTFRegion(env, str, 0, str_len, cstr);
	return xpn_chdir(cstr);
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1chmod
  (JNIEnv *env, jobject obj, jstring path, jshort mode){

	int path_len = (*env)->GetStringLength(env, path);
	char cpath[path_len + 1];
	(*env)->GetStringUTFRegion(env, path, 0, path_len, cpath);
	return xpn_chmod(cpath, (mode_t) mode);
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1close
  (JNIEnv *env, jobject obj, jint fd){
    return xpn_close(fd);
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1creat
  (JNIEnv *env, jobject obj, jstring path, jlong mode){

	int path_len = (*env)->GetStringLength(env, path);
	char cpath[path_len + 1];
	(*env)->GetStringUTFRegion(env, path, 0, path_len, cpath);
	return xpn_creat(cpath, (mode_t) mode);
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1destroy
  (JNIEnv *env, jobject obj){
    return xpn_destroy();
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1exist
  (JNIEnv *env, jobject obj, jstring path){

	int path_len = (*env)->GetStringLength(env, path);
	char cpath[path_len + 1];
	(*env)->GetStringUTFRegion(env, path, 0, path_len, cpath);
	struct stat stats;
	int i = xpn_stat(cpath, &stats);

	if (i != 0) return -1;

	return 0;

  }

JNIEXPORT jobject JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1fstat
  (JNIEnv *env, jobject obj, jint fd){
	
	struct stat stats;
	jobject jstats;
	
	xpn_fstat(fd, &stats);
	parseXPNStat(env, &stats, &jstats);

	return jstats;
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1get_1block_1locality
  (JNIEnv *env, jobject obj, jstring jpath, jlong joffset, jobjectArray jurl_v) {
	
    const char *path = (*env)->GetStringUTFChars(env, jpath, NULL);

    off_t offset = (off_t) joffset;

    jsize url_v_length = (*env)->GetArrayLength(env, jurl_v);
	int url_c;

    char **url_v_c = (char **)malloc(url_v_length * sizeof(char *));
    int result = xpn_get_block_locality((char *)path, offset, &url_c, (char ***)&url_v_c);

    for (int i = 0; i < url_v_length; i++) {
        if (url_v_c[i] != NULL) {
            jstring newString = (*env)->NewStringUTF(env, url_v_c[i]);

            (*env)->SetObjectArrayElement(env, jurl_v, i, newString);

            free(url_v_c[i]);
        }
    }

    free(url_v_c);
    (*env)->ReleaseStringUTFChars(env, jpath, path);

    return result;

  }

JNIEXPORT jstring JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1getcwd
  (JNIEnv *env, jobject obj, jstring path, jlong size){

	int path_len = (*env)->GetStringLength(env, path);
	char cpath[path_len + 1];
	(*env)->GetStringUTFRegion(env, path, 0, path_len, cpath);
	return (*env)->NewStringUTF(env, xpn_getcwd(cpath, size));  
}

JNIEXPORT jobjectArray JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1getDirContent
  (JNIEnv *env, jobject obj, jstring path){

	int path_len = (*env)->GetStringLength(env, path);
	char cpath[path_len + 1];
	(*env)->GetStringUTFRegion(env, path, 0, path_len, cpath);

	int size = 0;
	DIR *dir = xpn_opendir(cpath);
	struct dirent *dir_list;

	while (xpn_readdir(dir) != NULL){
		size++;
	}

	xpn_closedir(dir);

	jobjectArray res = (jobjectArray) (*env)->NewObjectArray(env, size, (*env)->FindClass(env, "java/lang/String"), NULL);
	dir = xpn_opendir(cpath);
	int i = 0;
	while ((dir_list = xpn_readdir(dir)) != NULL){
		(*env)->SetObjectArrayElement(env, res, i, (*env)->NewStringUTF(env, dir_list->d_name));
		i++;
	}

	xpn_closedir(dir);

	return res;
}

JNIEXPORT jstring JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1getGroupname
  (JNIEnv *env, jobject obj, jint id){
	
	char groupname[MAX_GR_NAME + 1];
	struct group *gr = getgrgid(id);

	strcpy(groupname, gr->gr_name);

	return (*env)->NewStringUTF(env, groupname);
}

JNIEXPORT jstring JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1getUsername
  (JNIEnv *env, jobject obj, jint id){

	char username[MAX_USR_NAME + 1];
	struct passwd *usr = getpwuid(id);

	strcpy(username, usr->pw_name);

	return (*env)->NewStringUTF(env, username);
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1init
  (JNIEnv *env, jobject obj){
	return xpn_init();
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1isDir
  (JNIEnv *env, jobject obj, jint mode){

	return S_ISDIR(mode);
}

JNIEXPORT jlong JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1lseek
  (JNIEnv *env, jobject obj, jint fd, jlong offset, jlong mode){
	return xpn_lseek(fd, (off_t) offset, (int) mode);
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1mkdir
  (JNIEnv *env, jobject obj, jstring src, jshort mode){

	int src_len = (*env)->GetStringLength(env, src);
	char csrc[src_len + 1];
	(*env)->GetStringUTFRegion(env, src, 0, src_len, csrc);

	return xpn_mkdir(csrc, (mode_t) mode);
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1open
  (JNIEnv *env, jobject obj, jstring path, jlong flags){

	int path_len = (*env)->GetStringLength(env, path);
	char cpath[path_len + 1];
	(*env)->GetStringUTFRegion(env, path, 0, path_len, cpath);
	return xpn_open(cpath, (int) flags);
}

JNIEXPORT jlong JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1read
  (JNIEnv *env, jobject obj, jint fd, jobject buf, jlong bufsize){

	void * buf_addr = (*env)->GetDirectBufferAddress(env, buf);
        
	return xpn_read(fd, buf_addr, (size_t) bufsize);
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1rename
  (JNIEnv *env, jobject obj, jstring src, jstring dst){

	int src_len = (*env)->GetStringLength(env, src);
	char csrc[src_len + 1];
	(*env)->GetStringUTFRegion(env, src, 0, src_len, csrc);
	int dst_len = (*env)->GetStringLength(env, dst);
	char cdst[dst_len + 1];
	(*env)->GetStringUTFRegion(env, dst, 0, dst_len, cdst);

	return xpn_rename(csrc, cdst);
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1rmdir
  (JNIEnv *env, jobject obj, jstring path){

	int path_len = (*env)->GetStringLength(env, path);
	char cpath[path_len + 1];
	(*env)->GetStringUTFRegion(env, path, 0, path_len, cpath);

	return xpn_rmdir(cpath);
}

JNIEXPORT jobject JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1stat
  (JNIEnv *env, jobject obj, jstring path){
	
	int path_len = (*env)->GetStringLength(env, path);
	char cpath[path_len + 1];
	(*env)->GetStringUTFRegion(env, path, 0, path_len, cpath);
	struct stat stats;
	jobject jstats;
        
	int i = xpn_stat(cpath, &stats);
	if (i != 0) return NULL;
	parseXPNStat(env, &stats, &jstats);

	return jstats;
}

JNIEXPORT jint JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1unlink
  (JNIEnv *env, jobject obj, jstring path){

	int path_len = (*env)->GetStringLength(env, path);
	char cpath[path_len + 1];
	(*env)->GetStringUTFRegion(env, path, 0, path_len, cpath);

	return xpn_unlink(cpath);
}

JNIEXPORT jlong JNICALL Java_org_expand_jni_ExpandToPosix_jni_1xpn_1write
  (JNIEnv *env, jobject obj, jint fd, jobject buf, jlong count){

	void * buf_addr = (*env)->GetDirectBufferAddress(env, buf);

	return xpn_write(fd, buf_addr, count);
}

