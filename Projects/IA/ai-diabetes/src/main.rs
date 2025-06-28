use ai_diabetes::Aux;
use ai_diabetes::ModelKind;
use csv::ReaderBuilder;
use linfa::prelude::*;
#[allow(unused_imports)]
use linfa_preprocessing::norm_scaling::NormScaler;
use ndarray::s;
use ndarray::{Array1, Array2, ArrayView, Axis};
use std::num::ParseFloatError;

const CSV_PATH: &str = "dataset.csv";

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // 1. Data loading and type conversion =====================================
    let columns: Vec<&'static str> = vec![
        "Gender", "AGE", "Urea", "Cr", "HbA1c", "Chol", "TG", "HDL", "LDL", "VLDL", "BMI",
    ];

    let mut reader = ReaderBuilder::new()
        .has_headers(true)
        .delimiter(b',')
        .from_path(CSV_PATH)
        .expect("can create reader");

    let mut data: Array2<f64> = Array2::zeros((0, 11));
    let mut targets: Array1<bool> = Array1::default(0);
    for result in reader.records() {
        let record = result.expect("Error reading record");
        let row: Vec<String> = record.iter().map(|s| s.to_string()).collect();

        // Note: The first two columns (Id, No_Pation) are not used in the model
        let mut filtered_row = row[2..].to_vec();

        // Convert "Gender" to "bool" casted as f64
        match filtered_row[0].as_str().trim() {
            "M" | "m" => filtered_row[0] = "1".to_string(),
            "F" | "f" => filtered_row[0] = "0".to_string(),
            _ => {}
        }

        // Convert "Class" to "bool" casted as f64
        match filtered_row[11].as_str().trim() {
            "N" | "n" => filtered_row[11] = "0".to_string(),
            "P" | "p" => filtered_row[11] = "0".to_string(),
            "Y" | "y" => filtered_row[11] = "1".to_string(),
            _ => {}
        }

        let parsed_row = filtered_row
            .iter()
            .map(|s| s.trim().parse::<f64>())
            .collect::<Result<Vec<f64>, ParseFloatError>>()
            .expect("Error parsing row: a value could not be converted to float");

        data.append(
            Axis(0),
            ArrayView::from_shape((1, 11), &parsed_row[0..11]).unwrap(),
        )
        .expect("Error appending row to data");
        targets
            .append(
                Axis(0),
                ArrayView::from_shape(1, &[((parsed_row[11] - 1.0).abs() <= f64::EPSILON)])
                    .unwrap(),
            )
            .expect("Error appending row to targets");
    }

    // 2. Data cleaning ========================================================

    // No action needed (see notebook)

    // 3. Data preprocessing ===================================================

    // 3.1. Oversampling non-diabetic rows
    let mut non_diabetic_indices = Vec::new();
    for (i, &target) in targets.iter().enumerate() {
        if !target {
            non_diabetic_indices.push(i);
        }
    }

    let diabetic_count = targets.iter().filter(|&&t| t).count();
    let non_diabetic_count = non_diabetic_indices.len();
    let oversample_count = diabetic_count - non_diabetic_count;

    for _ in 0..oversample_count {
        let random_index = non_diabetic_indices[rand::random::<usize>() % non_diabetic_count];
        let new_row = data.slice(s![random_index, ..]).to_owned().to_vec();
        let new_target = targets[random_index];
        data.append(Axis(0), ArrayView::from_shape((1, 11), &new_row).unwrap())
            .expect("Error appending oversampled row");
        targets
            .append(Axis(0), ArrayView::from_shape(1, &[new_target]).unwrap())
            .expect("Error appending oversampled target");
    }

    // 3.2. Normalization

    /* // <- Remove here the comment to use L1 normalization
    let temp_dataset = Dataset::new(data.clone(), targets.clone()).with_feature_names(columns.clone());
    let scaler = NormScaler::l1();
    let temp_dataset = scaler.transform(temp_dataset);
    let data = temp_dataset.records().to_owned();
    let targets = temp_dataset.targets().to_owned();
    */ // <-

    // 3.3. Feature creation for highly correlated columns

    // Print a correlation heatmap
    let temp_dataset =
        Dataset::new(data.clone(), targets.clone()).with_feature_names(columns.clone());
    let correlation = temp_dataset.pearson_correlation();
    println!("{}", correlation);

    // 4. Model creation and results ===========================================

    let dataset = Dataset::new(data.clone(), targets.clone()).with_feature_names(columns.clone());
    let dataset = dataset.shuffle(&mut rand::thread_rng());
    let (train_data, test_data) = dataset.split_with_ratio(0.8);

    // 4.1. Support Vector Machine

    let svm = Aux::train_and_test(ModelKind::SVM, &train_data, &test_data);
    println!("{}", svm);

    // 4.2. Gaussian Naive Bayes

    let gnb = Aux::train_and_test(ModelKind::GNB, &train_data, &test_data);
    println!("{}", gnb);

    // 4.3. (Linear) Decision Trees

    let ldt = Aux::train_and_test(ModelKind::LDT, &train_data, &test_data);
    println!("{}", ldt);

    Ok(())
}
