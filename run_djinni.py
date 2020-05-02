#!/usr/bin/python
# This script generates language interfaces from djinni IDL files

import os
import shutil
import sys

script_dir = os.path.dirname(os.path.realpath(__file__))
base_dir = os.path.normpath(script_dir)
djinni_dir = base_dir + "/common/djinni"

api_in = djinni_dir + "/main.djinni"
api_temp_out = base_dir + "/scripts/djinni-output-api-temp"
api_java_package = "org.example.xptuto"
api_gen_stamp = api_temp_out + "/gen.api.stamp"
api_cpp_out = base_dir + "/common/src/gen"
api_cpp_namespace = "xptuto"
api_java_out = base_dir + "/android/app/src/main/gen_java/org/example/xptuto"
api_jni_out = base_dir + "/android/app/src/main/cpp/gen"

# Clean command-line argument
if len(sys.argv) > 1:
    command = sys.argv[1]
    if command != "clean":
        print ("Unexpected command: " + command)
        sys.exit(1)
    else:
        for folder in [api_temp_out, api_cpp_out, api_java_out, api_jni_out]:
            print ("Deleting " + folder)
            shutil.rmtree(folder, ignore_errors=True)
        sys.exit(0)

# Build djinni
os.system(base_dir + "/deps/djinni/src/build")

# Generate
djinni_cmd = ("{base_dir}/deps/djinni/src/run-assume-built "
              "--cpp-out {temp_out}/cpp "
              "--cpp-namespace {cpp_namespace} "
              "--java-out {temp_out}/java "
              "--java-package {java_package} "
              "--jni-out {temp_out}/jni "
              "--idl {in_file} ")

def mirror(prefix, from_path, to_path):
    if not os.path.exists(to_path):
        os.makedirs(to_path)
    os.system("rsync -a --delete --checksum --itemize-changes %s/ %s | grep -v '^\.' | sed \"s/^/[%s]\"/" % (
    from_path, to_path, prefix))


# Build djinni api files
if os.path.exists(api_in):
    print ("Build api files")
    djinni_cmd_api = djinni_cmd.format(base_dir=base_dir, temp_out=api_temp_out, cpp_namespace=api_cpp_namespace,
                                       java_package=api_java_package, in_file=api_in)
    os.system(djinni_cmd_api)
    mirror("cpp", api_temp_out + "/cpp", api_cpp_out)
    mirror("java", api_temp_out + "/java", api_java_out)
    mirror("jni", api_temp_out + "/jni", api_jni_out)
    os.system("date > " + api_gen_stamp)
else:
    print ("Error:" + api_in + "not found!")

print ("Done.")
