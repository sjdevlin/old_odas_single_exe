    
    #include "cmatrix.h"

    cmatrix_obj * cmatrix_construct_zero(const unsigned int nRows, const unsigned int nCols) {

        cmatrix_obj * obj;

        obj = (cmatrix_obj *) malloc(sizeof(cmatrix_obj));

        obj->nRows = nRows;
        obj->nCols = nCols;

        obj->real = (float *) malloc(sizeof(float) * nRows * nCols);
        obj->imag = (float *) malloc(sizeof(float) * nRows * nCols);

        return obj;

    }

    cmatrix_obj * cmatrix_construct_matrix(const cmatrix_obj * cmatrix) {

        cmatrix_obj * obj;

        obj = (cmatrix_obj *) malloc(sizeof(cmatrix_obj));

        obj->nRows = cmatrix->nRows;
        obj->nCols = cmatrix->nCols;

        memcpy(obj->real, cmatrix->real, sizeof(float) * cmatrix->nRows * cmatrix->nCols);
        memcpy(obj->imag, cmatrix->imag, sizeof(float) * cmatrix->nRows * cmatrix->nCols);

        return obj;

    }

    void cmatrix_copy_zero(cmatrix_obj * obj) {

        memset(obj->real, 0x00, sizeof(float) * obj->nRows * obj->nCols);
        memset(obj->imag, 0x00, sizeof(float) * obj->nRows * obj->nCols);

    }

    void cmatrix_copy_cmatrix(cmatrix_obj * obj, const cmatrix_obj * cmatrix) {

        memcpy(obj->real, cmatrix->real, sizeof(float) * cmatrix->nRows * cmatrix->nCols);
        memcpy(obj->imag, cmatrix->imag, sizeof(float) * cmatrix->nRows * cmatrix->nCols);

    }

    void cmatrix_destroy(cmatrix_obj * obj) {

        free((void *) obj->real);
        free((void *) obj->imag);
        free((void *) obj);

    }

    void cmatrix_transpose(cmatrix_obj * obj, const cmatrix_obj * src) {

        unsigned int iRow;
        unsigned int iCol;

        for (iRow = 0; iRow < obj->nRows; iRow++) {

            for (iCol = 0; iCol < obj->nCols; iCol++) {

                obj->real[iRow * obj->nRows + iCol] = src->real[iCol * obj->nRows + iRow];
                obj->imag[iRow * obj->nRows + iCol] = src->imag[iCol * obj->nRows + iRow];

            }

        }

    }

    void cmatrix_hermitian(cmatrix_obj * obj, const cmatrix_obj * src) {

        unsigned int iRow;
        unsigned int iCol;

        for (iRow = 0; iRow < obj->nRows; iRow++) {

            for (iCol = 0; iCol < obj->nCols; iCol++) {

                obj->real[iRow * obj->nRows + iCol] = src->real[iCol * obj->nRows + iRow];
                obj->imag[iRow * obj->nRows + iCol] = -1.0f * src->imag[iCol * obj->nRows + iRow];

            }

        }

    }

    void cmatrix_add(cmatrix_obj * obj, const cmatrix_obj * src1, const cmatrix_obj * src2) {

        unsigned int iRow;
        unsigned int iCol;

        for (iRow = 0; iRow < obj->nRows; iRow++) {

            for (iCol = 0; iCol < obj->nCols; iCol++) {

                obj->real[iRow * obj->nRows + iCol] = src1->real[iRow * obj->nRows + iCol] + src2->real[iRow * obj->nRows + iCol];

            }

        }

    }

    void cmatrix_sub(cmatrix_obj * obj, const cmatrix_obj * src1, const cmatrix_obj * src2) {

        unsigned int iRow;
        unsigned int iCol;

        for (iRow = 0; iRow < obj->nRows; iRow++) {

            for (iCol = 0; iCol < obj->nCols; iCol++) {

                obj->real[iRow * obj->nRows + iCol] = src1->real[iRow * obj->nRows + iCol] - src2->real[iRow * obj->nRows + iCol];

            }

        }

    }

    void cmatrix_mul(cmatrix_obj * obj, const cmatrix_obj * src1, const cmatrix_obj * src2) {

        unsigned int iRow;
        unsigned int iCol;
        unsigned int iElement;

        float real1, imag1;
        float real2, imag2;
        float real12, imag12;

        for (iRow = 0; iRow < src1->nRows; iRow++) {

            for (iCol = 0; iCol < src2->nCols; iCol++) {

                obj->real[iRow*obj->nRows+iCol] = 0.0f;
                obj->imag[iRow*obj->nRows+iCol] = 0.0f;

                for (iElement = 0; iElement < src1->nCols; iElement++) {

                    real1 = src1->real[iRow*obj->nRows+iElement];
                    imag1 = src1->imag[iRow*obj->nRows+iElement];
                    real2 = src2->real[iElement*obj->nRows+iCol];
                    imag2 = src2->imag[iElement*obj->nRows+iCol];

                    real12 = real1 * real2 - imag1 * imag2;
                    imag12 = real1 * imag2 + imag1 * real2;

                    obj->real[iRow*obj->nRows+iCol] += real12;
                    obj->imag[iRow*obj->nRows+iCol] += imag12;

                }

            }

        }

    }

    void cmatrix_scale(cmatrix_obj * obj, const cmatrix_obj * src, const float scale) {

        unsigned int iRow;
        unsigned int iCol;

        for (iRow = 0; iRow < src->nRows; iRow++) {

            for (iCol = 0; iCol < src->nCols; iCol++) {

                obj->real[iRow * obj->nRows + iCol] = scale * src->real[iRow * obj->nRows + iCol];
                obj->imag[iRow * obj->nRows + iCol] = scale * src->imag[iRow * obj->nRows + iCol];

            }

        }

    }

    void cmatrix_printf(const cmatrix_obj * obj) {

        unsigned int iRow;
        unsigned int iCol;

        for (iRow = 0; iRow < obj->nRows; iRow++) {

            for (iCol = 0; iCol < obj->nCols; iCol++) {

                printf("(%+1.5f + %+1.5fj) ",obj->real[iRow*obj->nRows+iCol],obj->imag[iRow*obj->nRows+iCol]);

            }

            printf("\n");

        }

    }