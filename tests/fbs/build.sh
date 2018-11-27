cd `dirname $0`
chmod +x ./flatc/flatc_mac

./flatc/flatc_mac --cpp test.fbs
./flatc/flatc_mac --python test.fbs
fbs_reader test.fbs

mv -f *_generated.h ../cpp/
mv -f *_reader.h ../cpp/
cp -rf flatbuffers ../cpp/

mv -f *_reader.py ../py3/
mv -f *_fbs/* ../py3/
cp -rf flatbuffers ../py3/





echo fbs build success!