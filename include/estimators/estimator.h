// Copyright (C) 2019 Czech Technical University.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of Czech Technical University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Please contact the author of this library if you have any questions.
// Author: Daniel Barath (barath.daniel@sztaki.mta.hu)
#pragma once

#include <vector>

namespace estimator {
    // Templated class for estimating a model for RANSAC.
    // This class is purely a virtual class and should be implemented for the specific task  that RANSAC is being used for.
    // Two methods must be implemented: estimateModel and residual.
    // All other methods are optional, but will likely enhance the quality of the RANSAC output.
    template<typename DatumType, typename ModelType>
    class Estimator {
    public:
        typedef DatumType Datum;
        typedef ModelType Model;

        Estimator() = default;

        virtual ~Estimator() = default;

        /*!
         * Given a minimal set of data points, estimate the model.
         * @param data Given a set of data points
         * @param sample The selected sample which will be used for estimation
         * @param model The estimated model parameters
         * @return Returns true for successful model estimation (and outputs model), false for failed estimation.
         */
        virtual bool estimateModel(const Datum &data, const size_t *sample, std::vector<Model> *model) const = 0;

        // Estimate a model from a non-minimal sampling of the data.
        // By default, this simply implements the minimal case.
        // In case of weighted least-squares, the weights can be fed into the function.
        virtual bool estimateModelNonMinimal(const Datum &data, const size_t *sample, const size_t &sample_number,
                                             std::vector<Model> *model, const double *weights_) const = 0;

        // Given a model and a data point, calculate the error.
        // Users should implement this function appropriately for the task being solved.
        virtual double residual(const Datum &data, const Model &model) const = 0;

        virtual double squaredResidual(const Datum &data, const Model &model) const = 0;

        // Enable a quick check to see if the model is valid. This can be a geometric
        // check or some other verification of the model structure.
        inline virtual bool isValidModel(const Model &model) const { return true; }

        // Enable a quick check to see if the model is valid. This can be a geometric
        // check or some other verification of the model structure.
        inline virtual bool isValidModel(Model &model, const Datum &data, const std::vector<size_t> &inliers,
                                         const size_t *minimal_sample_,
                                         const double threshold_,
                                         bool &model_updated_) const { return true; }
    };
}
