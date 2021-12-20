#pragma once
#include <opencv2/opencv.hpp>

namespace im_param {

    template<
        typename backend_type,
        class T,
        int Dim, 
        std::enable_if_t<Dim <= 4, bool> = true
    >
    backend_type& parameter(
        backend_type& backend, 
        const std::string& name, 
        cv::Vec<T,Dim>& value, 
        cv::Vec<T,Dim> min = cv::Vec<T,Dim>(static_cast<T>(0)),
        cv::Vec<T,Dim> max = cv::Vec<T,Dim>(static_cast<T>(1)),
        cv::Vec<T,Dim> scale = cv::Vec<T,Dim>(static_cast<T>(1))
    )
    {
        parameter(backend, name, &value[0], Dim, &min[0], &max[0], &scale[0]);
        return backend;
    }

    template<
        class T, 
        int Columns,
        int Rows
    >
    cv::Matx<T, Rows, Columns> opencv_mat_with_value(T value)
    {
        cv::Matx<T, Rows, Columns> result;
        for (int j = 0; j < Rows; ++j)
        {
            for (int i = 0; i < Columns; ++i)
            {
                result(j,i) = value;
            }
        }
        return result;
    }

    template<
        typename backend_type,
        class T,
        int Columns,
        int Rows 
        // std::enable_if_t<Columns <= 4 && Rows <= 4, bool> = true
    >
    backend_type& parameter(
        backend_type& backend, 
        const std::string& name, 
        cv::Matx<T, Rows, Columns>& value, 
        cv::Matx<T, Rows, Columns> min = opencv_mat_with_value<T, Rows, Columns>(static_cast<typename T>(0)),
        cv::Matx<T, Rows, Columns> max = opencv_mat_with_value<T, Rows, Columns>(static_cast<typename T>(1)),
        cv::Matx<T, Rows, Columns> scale = opencv_mat_with_value<T, Rows, Columns>(static_cast<typename T>(1))
    )
    {
        for (int j = 0; j < Rows; ++j)
        {
            auto* row = &value(j,0);
            auto* row_min = &min(j,0);
            auto* row_max = &max(j,0);
            auto* row_scale = &scale(j,0);
            
            std::string row_name = name + std::string("[") + std::to_string(j) + std::string("]");
            parameter(backend, row_name, row, Columns, row_min, row_max, row_scale);
        }
        return backend;
    }

} // namespace im_param
