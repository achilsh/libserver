#ifndef DATABASE_H_
#define DATABASE_H_

// 数据库存储
#include <stdint.h>
#include <mysql++/mysql++.h>

#include <memory.h>
#include <string>
#include <vector>
#include <stdint.h>

#include "machine_study.pb.h"

using namespace std;
using namespace mysqlpp;

class MySqlDatabase {
    public:
        MySqlDatabase();
        ~MySqlDatabase();

        // 初始化数据库
        bool initDatabase();

        // 插入二进制数据
        bool insert2(const std::string& bytes);
        bool insert(int64_t st_id, int64_t picture_bin_id, const std::string & dbcode, 
                int32_t tonality1, int32_t tonality2, int32_t tonality3, 
                const std::string & feature_data);

        // 从数据库中读取数据
        bool read(int64_t index, int64_t max, std::vector<std::shared_ptr<mstudy::FeatureData>>& datas);
    private:
        bool is_connected_;
        // mysql connector
        std::shared_ptr<Connection> conn_; 
};

#endif  
