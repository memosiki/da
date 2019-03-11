source=lab2.cpp
bin=patricia



for test_file in `ls tests/*.t`; do
    echo "Execute ${test_file}"
    if ! ./${bin} < $test_file > tmp ; then
        echo "ERROR"
        continue
    fi
    answer_file="${test_file%.*}"

    if ! diff -u "${answer_file}.txt" tmp ; then
        echo "Failed"
    else
        echo "OK"
    fi 
done
