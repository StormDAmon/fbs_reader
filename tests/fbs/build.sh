cd `dirname $0`

cp ../../build/src/fbs_reader ./flatc/fbs_reader
chmod +x ./flatc/fbs_reader
chmod +x ./flatc/flatc_mac

./flatc/flatc_mac --cpp test.fbs
./flatc/flatc_mac --python test.fbs
./flatc/fbs_reader test.fbs

rm -rf ../cpp/*_generated.h
mv -f *_generated.h ../cpp/
rm -rf ../cpp/*_reader.h
mv -f *_reader.h ../cpp/
rm -rf ../cpp/flatbuffers
cp -rf flatbuffers ../cpp/

rm -rf ../py3/_reader.py
mv -f *_reader.py ../py3/
rm -rf ../py3/*_fbs
mv -f *_fbs ../py3/
rm -rf ../py3/flatbuffers
cp -rf flatbuffers ../py3/





echo fbs build success!