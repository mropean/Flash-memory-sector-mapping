#pragma once

#include <iostream>				// 일반적인 c++ 라이브러리
#include <fstream>				// 파일입출력용 c++ 라이브러리
#include <cstdio>				// c 형식으로 사용할때 stdio.h와 같은 기능을 하는 라이브러리
#include <string>				// c++ 형식 문자열을 가지고 놀 때 사용하는 라이브러리
using namespace std;

void input_Command();		// 명령어 식별 함수
void init(int mb_Size);		// 메모리 생성, 초기화 함수
void Print_table();			// 매핑 테이블 출력 함수.
void FTL_read(int sector_num);		// FTL 로드 함수
void FTL_write(int sector_num, char* data);	// FTL 입력 함수
void Flash_write(int sector_num, char* data);	// Flash 입력 함수
void Flash_read(int sector_num);				// Flash 로드 함수
void Flash_erase(int block_num);				// Flash 지우기 함수
void Spare_erased();							// 스페어 블록 초기화 함수(지우기 연산)
int sector_mapping(int sector_num);				// 미 사용 / 섹터가 파일의 몇번째 줄에 있는지 확인용