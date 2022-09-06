#pragma once

#include <iostream>				// �Ϲ����� c++ ���̺귯��
#include <fstream>				// ��������¿� c++ ���̺귯��
#include <cstdio>				// c �������� ����Ҷ� stdio.h�� ���� ����� �ϴ� ���̺귯��
#include <string>				// c++ ���� ���ڿ��� ������ �� �� ����ϴ� ���̺귯��
using namespace std;

void input_Command();		// ��ɾ� �ĺ� �Լ�
void init(int mb_Size);		// �޸� ����, �ʱ�ȭ �Լ�
void Print_table();			// ���� ���̺� ��� �Լ�.
void FTL_read(int sector_num);		// FTL �ε� �Լ�
void FTL_write(int sector_num, char* data);	// FTL �Է� �Լ�
void Flash_write(int sector_num, char* data);	// Flash �Է� �Լ�
void Flash_read(int sector_num);				// Flash �ε� �Լ�
void Flash_erase(int block_num);				// Flash ����� �Լ�
void Spare_erased();							// ����� ��� �ʱ�ȭ �Լ�(����� ����)
int sector_mapping(int sector_num);				// �� ��� / ���Ͱ� ������ ���° �ٿ� �ִ��� Ȯ�ο�