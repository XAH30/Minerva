#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <set>
#include <cassert>
#include <chrono>
#include <thread>
#include <locale>
#include <utility>
#include <functional>
#include <filesystem>
#include <cctype>
#include <numeric>

using namespace std;
namespace fs = std::filesystem;

/*
ПОДГРУЗКА ФУНКЦИОНАЛА РАБОТЫ С ДАННЫМИ
*/

// Класс для работы с результатом
#include "Classes/Results.cpp"

// Кастомное считывание из файла
#include "unfilteredFunctions/contains.cpp"
#include "unfilteredFunctions/starts_with.cpp"
#include "unfilteredFunctions/ends_with.cpp"
#include "unfilteredFunctions/read_functions_from_file.cpp"
#include "unfilteredFunctions/read_functions.cpp"

// Функции вывода разных данных
#include "DataPrinters/allDataPrinter.cpp"
#include "DataPrinters/printVector.cpp"
#include "DataPrinters/oneHotMatrixPrinter.cpp"
#include "DataPrinters/bucketPrinter.cpp"
#include "DataPrinters\candidatesPrinter.cpp"

// Функция для получения вектора, состоящего из столбца или строки передаваемых данных 
#include "iFunctions/iloc.cpp"

// Функция для заполнения диапазона элементами, последовательно увеличивая их значения на заданное значение
#include "iFunctions/iota.cpp"

/*
ПОДГРУЗКА ФУНКЦИОНАЛА ДЛЯ АЛГОРИТМА LSH
*/

// Функции хэширования
#include "hashFunctions/minHash.cpp"
#include "hashFunctions/simHash.cpp"

// Остальные функции
#include "unfilteredFunctions/buildShingles.cpp"
#include "unfilteredFunctions/buildVocab.cpp"
#include "unfilteredFunctions/oneHotEncoder.cpp"
#include "unfilteredFunctions/getSignature.cpp"
#include "unfilteredFunctions/transpose.cpp"
#include "unfilteredFunctions/preparations.cpp"
#include "unfilteredFunctions/close.cpp"
#include "unfilteredFunctions/vec2set.cpp"
#include "unfilteredFunctions/set2vec.cpp"

// класс LSH
#include "Classes/LSH.cpp"

// Поиск кандидатов на схожесть
#include "unfilteredFunctions/candidatesFinder.cpp"

/*
ПОДГРУЗКА ФУНКЦИОНАЛА ДЛЯ АЛГОРИТМА FINESSE
*/

#include "unfilteredFunctions/isSimilar.cpp"
#include "unfilteredFunctions/splitIntoSubchunks.cpp"

// Алгоритм Finesse
#include "unfilteredFunctions\Finesse.cpp"


int main(){
    preparations();
    // Уязвимость функции do_submit_urb() в модуле drivers/media/usb/siano/smsusb.c драйвера цифрового 
    // ТВ siano ядра операционной системы Linux связана с использованием ранее освобожденной памяти. 
    // Эксплуатация уязвимости может позволить нарушителю вызвать отказ в обслуживании.

    //Уязвимость функции cxgb4_cleanup_tc_flower() в модуле drivers/net/ethernet/chelsio/cxgb4/cxgb4_tc_flower.c 
    // драйвера Chelsio cxgb4 ядра операционной системы Linux связана с повторным использованием ранее освобожденной памяти. 
    // Эксплуатация уязвимости может позволить нарушителю вызвать отказ в обслуживании.
    auto startFinesse = chrono::steady_clock::now();
    vector<string> data = read_functions("..\\linux-master");
    std::set<std::string> dataset = vec2set(data);
    data = set2vec(dataset);

    std::vector<std::string> sentence = {data[554907], data[660595]}; 
    size_t subchunkSize = 16;
    double similarityThreshold = 0.6; // Степень похожести в долях от 1
    set<pair<int, int>> candidate_pairs3 = Finesse(sentence, data, subchunkSize, similarityThreshold);
    auto endFinesse = chrono::steady_clock::now();

    Results FinesseResults(candidate_pairs3, "Finesse");
    int num_pairs3 = candidate_pairs3.size();
    std::cout << std::endl << "Number of candidate pairs (using Finesse): " << num_pairs3 << std::endl;
    std::cout << "Finesse elapsed time in nanoseconds: "<< chrono::duration_cast<chrono::nanoseconds>(endFinesse - startFinesse).count() << " ns" << std::endl << std::endl;

    vector<pair<int, int>> resultVector(candidate_pairs3.begin(), candidate_pairs3.end());
    for(int i=0; i<resultVector.size(); i++){
        std::cout << "\nSimilar data:\n\n" << sentence[resultVector[i].first] << 
       "\n----------------------------------------\n" << data[resultVector[i].second] << "\n\nEnd of Similar data.\n";
    }
    close();
}