//    This file is part of ComputerExerciseProblems
//    Copyright (C) 2021  IllusionAutumn

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef FILTERILLUSION_H
#define FILTERILLUSION_H
#include"source.hpp"

class filterIllusion
{
public:
    filterIllusion();

    /**
     * @brief Deduplication 删除重复数据
     * @return 返回删除数据的条数
     */
    qsizetype Deduplication(QString saveName,bool isUSEForce,bool isR);

    void MergeFile(QString regStr,QString findStr,QString shuffle,QString saveName,bool isRemove,bool isNum);

private:
    IllusionFile* _It;
};

#endif // FILTERILLUSION_H
