// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include <exception>
#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{

    if (len < 1) {
        std::exception err("No correct lenght");
        throw err;
    }
    BitLen = len;
    MemLen = (BitLen - 1) / (sizeof(TELEM) * 8 )+ 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[ i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8); ;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    int MemIndex = this->GetMemIndex(n);
    int BitMemIndex = n - ((sizeof(TELEM) * 8) * MemIndex);
    TELEM a;
    a=1<< (31 - BitMemIndex);
    TELEM Mask = pMem[MemIndex] & a;
    return Mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0)||(n>=BitLen)) {
        std::exception err("No correct index");
        throw err;
    }
    int MemIndex = this->GetMemIndex(n);
    int BitMemIndex = n - ((sizeof(TELEM) * 8) * MemIndex);
    TELEM a = 1;
    a=a << (31 - BitMemIndex);
    pMem[MemIndex] = pMem[MemIndex]| a;
    
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n >= BitLen)) {
        std::exception err("No correct index");
        throw err;
    }
    int MemIndex = this->GetMemIndex(n);
    TELEM mask = this->GetMemMask(n);
    pMem[MemIndex]=pMem[MemIndex] ^ mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n >= BitLen)) {
        std::exception err("No correct index");
        throw err;
    }
    int MemIndex = this->GetMemIndex(n);
    int BitMemIndex = n - ((sizeof(TELEM) * 8) * MemIndex);
    TELEM a;
    int bit;
   a= 1<< (31 - BitMemIndex);
    if ((pMem[MemIndex] & a) == 0) {
        bit = 0;
    }
    else { bit = 1; }
    return bit;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        this->BitLen = bf.BitLen;
        this->MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
 int res = 0;
 if (this->BitLen == bf.BitLen) {
     res = 1;
     for (int i = 0; i < MemLen; i++) {
         res *=( this->pMem[i] == bf.pMem[i]);
     }
 }
  return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    int res = 0;
    if (this->BitLen != bf.BitLen) {
        res = 1;
    }
    else {
        for (int i = 0; i < MemLen; i++) {
            res += (this->pMem[i] != bf.pMem[i]);
        }
        if (res != 0) {
            res = 1;
        }
    }
    return res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int MaxLen;
    if (BitLen > bf.BitLen) {
        MaxLen = BitLen;
        TBitField Copyleast(MaxLen);
        for (int i = 0; i < bf.MemLen; i++) {
            Copyleast.pMem[i] = bf.pMem[i];
        }
        TBitField Copybiggest(*this);
        TBitField RESULT(MaxLen);
        for (int i = 0; i < RESULT.MemLen; i++) {
            RESULT.pMem[i] = (Copybiggest.pMem[i] | Copyleast.pMem[i]);
        }
        return RESULT;
    }
    else if (BitLen < bf.BitLen) {
        MaxLen = bf.BitLen;
        TBitField Copyleast(MaxLen);
        for (int i = 0; i < MemLen; i++) {
            Copyleast.pMem[i] = pMem[i];
        }
        TBitField RESULT(MaxLen);
        TBitField Copybiggest(bf);
        for (int i = 0; i < RESULT.MemLen; i++) {
            RESULT.pMem[i] = (Copybiggest.pMem[i] | Copyleast.pMem[i]);
        }
        return RESULT;
    }
    else {
        MaxLen = BitLen;
        TBitField Copyleast(*this);
        TBitField Copybiggest(bf);
        TBitField RESULT(MaxLen);
        for (int i = 0; i < RESULT.MemLen; i++) {
            RESULT.pMem[i] = (Copybiggest.pMem[i] |Copyleast.pMem[i]);
        }
        return RESULT;
    }

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int MaxLen;
    if (BitLen > bf.BitLen) {
       MaxLen = BitLen;
        TBitField Copyleast(MaxLen);
        for (int i = 0; i < bf.MemLen; i++) {
            Copyleast.pMem[i] = bf.pMem[i];
        }
        TBitField Copybiggest(*this);
        TBitField RESULT(MaxLen);
        for (int i = 0; i < RESULT.MemLen; i++) {
            RESULT.pMem[i] = (Copybiggest.pMem[i] & Copyleast.pMem[i]);
        }
        return RESULT;
   }
    else if(BitLen < bf.BitLen) {
         MaxLen = bf.BitLen;
        TBitField Copyleast(MaxLen);
        for (int i = 0; i <MemLen; i++) {
            Copyleast.pMem[i] = pMem[i];
        }
        TBitField RESULT(MaxLen);
        TBitField Copybiggest(bf);
        for (int i = 0; i < RESULT.MemLen; i++) {
            RESULT.pMem[i] = (Copybiggest.pMem[i] & Copyleast.pMem[i]);
        }
        return RESULT;
    }
    else {
        MaxLen = BitLen;
        TBitField Copyleast(*this);
        TBitField Copybiggest(bf);
        TBitField RESULT(MaxLen);
        for (int i = 0; i < RESULT.MemLen; i++) {
            RESULT.pMem[i] = (Copybiggest.pMem[i] & Copyleast.pMem[i]);
        }
        return RESULT;
    }
    

    
    
}

TBitField TBitField::operator~(void) // отрицание
{
    
    int a = BitLen - (sizeof(TELEM) * 8) * (MemLen - 1);
    TBitField RES(BitLen);
    for(int i=0;i<MemLen-1;i++){
        RES.pMem[i] = ~pMem[i];
    }
    for (int i = (sizeof(TELEM) * 8) * (MemLen - 1); i < BitLen; i++) {
        if (GetBit(i) == 1) {
            RES.ClrBit(i);
       }
        else {
            RES.SetBit(i);
        }
    }
    return RES;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int* mas = new int[bf.BitLen];
    for(int i=0;i<bf.BitLen;i++){
        istr >> mas[i];
    }
    for (int i = 0; i < bf.BitLen; i++) {
        if (mas[i] == 1) {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
 
    return ostr;
}

