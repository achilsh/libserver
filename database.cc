#include "database.h"

#include <fcntl.h>
#include <stdio.h>
#include <iostream>

#include <glog/logging.h>
#include <glog/stl_logging.h>

#include <google/protobuf/text_format.h>  
#include <google/protobuf/io/zero_copy_stream_impl.h>  

#include "machine_study_config.pb.h"
#include "machine_study.pb.h"

using namespace mstudy;

MySqlDatabase::MySqlDatabase() {
    is_connected_ = initDatabase();
}

MySqlDatabase::~MySqlDatabase() {
}

bool MySqlDatabase::initDatabase() {
    // 配置文件中读取数据库配置
    std::string file_name = "config.prototxt";
    // 读取文件
    int fd = open(file_name.c_str(), O_RDONLY);
    if (fd == -1) {
        LOG(ERROR) <<  "open " << file_name << " error";
        return false;
    }

    google::protobuf::io::FileInputStream* input = new google::protobuf::io::FileInputStream(fd);

    mstudy::MStudyConfig config;

    if (!google::protobuf::TextFormat::Parse(input, &config)){
        LOG(ERROR) << "textformat::parse error ";
    }

    delete input;

    close(fd);

    if (conn_ == NULL){
        conn_ = std::shared_ptr<mysqlpp::Connection>(new mysqlpp::Connection(config.db_name().c_str(), 
                    config.db_server().c_str(),     
                    config.db_username().c_str(),   
                    config.db_password().c_str(),  
                    config.db_port()));  
    }

   // conn_->set_option(new mysqlpp::SetCharsetDirOption("utf-8"));

    bool result = conn_->connected();

    LOG(INFO) << "connect " << config.db_server() << " " 
              << config.db_name() << " "
              << config.db_username() << " "
              << config.db_password() << " " 
              << config.db_port() << " " 
              << result;

    return result;
}


bool MySqlDatabase::insert2(const std::string& bytes){
    if (!is_connected_){
        LOG(ERROR) << "had not connect database." ;
        return false;
    }
    
    bool result = true;

    try {
        Query query = conn_->query();
        query << "insert into features(image_feature) values('"
          << mysqlpp::escape << bytes << "');";
        query.execute();
    }catch(const BadQuery& er){
        LOG(ERROR) << er.what(); 
        result = false;
    }catch(const Exception& er){
        LOG(ERROR) << er.what(); 
        result = false;
    }

    return result;
}

bool MySqlDatabase::insert(int64_t st_id, int64_t picture_bin_id, const std::string & dbcode, 
            int32_t tonality1, int32_t tonality2, int32_t tonality3, 
            const std::string & feature_data){
    if (!is_connected_){
        LOG(ERROR) << "had not connect database." ;
        return false;
    }
    
    bool result = true;

    LOG(INFO) << "insert data picture_bin_id :" << picture_bin_id;
    try {
        Query query = conn_->query();
        query << "insert into features(st_id, picture_bin_id, dbcode, tonality1, tonality2, tonality3, feature_data) values("
            << st_id << "," 
            << picture_bin_id << ","
            << "'" << mysqlpp::escape << dbcode << "',"
            << tonality1 << ","
            << tonality2 << ","
            << tonality3 << ","
            << "'" << mysqlpp::escape << feature_data << "');";

        query.execute();
    }catch(const BadQuery& er){
        LOG(ERROR) << er.what(); 
        result = false;
    }catch(const Exception& er){
        LOG(ERROR) << er.what(); 
        result = false;
    }

    return result;

}



bool MySqlDatabase::read(int64_t index, int64_t max, vector<shared_ptr<mstudy::FeatureData>>& datas){
    if (!is_connected_){
        LOG(ERROR) << "had not connect database." ;
    }
    bool result = true;

    try { 
        Query query = conn_->query();
        query << "select id, st_id, picture_bin_id, dbcode, tonality1, tonality2, tonality3, feature_data from features limit " << index <<"," << max;
        StoreQueryResult query_result = query.store();
        if (!query_result){
            LOG(ERROR) << query.error();
            return false;
        }

        int num_rows = query_result.num_rows();

        for (int i=0; i<num_rows; i++){
            shared_ptr<mstudy::FeatureData> item = shared_ptr<mstudy::FeatureData>(new mstudy::FeatureData);

            int64_t id = query_result[i][0];
            item->set_st_id(query_result[i][1]);
            item->set_picture_bin_id(query_result[i][2]);

            mysqlpp::sql_blob_null dbcode_blob = query_result[i][3];
            int count = dbcode_blob.data.length() / sizeof(float);
            float * dbcode = (float*)dbcode_blob.data.data();
            for(int i=0; i<count; i++){
                item->add_dbcode(dbcode[i]);
            }

            item->set_tonality1(query_result[i][4]);
            item->set_tonality2(query_result[i][5]);
            item->set_tonality3(query_result[i][6]);
            
            
            mysqlpp::sql_blob_null blob = query_result[i][7];
            if (blob.is_null){
                continue;
            }
            count = blob.data.length() / sizeof(float);
            float * array = (float*)blob.data.data();
            for (int i=0; i<count; i++){
               item->add_feature_data(array[i]); 
            }
            datas.push_back(item);

            LOG(INFO) << item->DebugString();
        }

    }catch(const BadQuery& er){
        LOG(ERROR) << er.what();
        result = false;
    }catch(const Exception& er){
        LOG(ERROR) << er.what();
        result = false;
    }

    return result;
}

















