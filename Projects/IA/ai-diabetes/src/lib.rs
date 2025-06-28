use linfa::prelude::*;
use linfa_bayes::GaussianNbParams;
use linfa_svm::Svm;
use linfa_trees::{DecisionTree, SplitQuality};
use ndarray::{ArrayBase, Dim, OwnedRepr};
use std::fmt::Display;

pub struct Aux;

/* const WIDTH: u32 = 800;
const HEIGHT: u32 = 400;
const MARGIN: i32 = 30;
const LABEL_SIZE: i32 = 30;
const BACKGROUND: &RGBColor = &WHITE; */

impl Aux {
    /* pub fn get_columns(table: &DataFrame) -> Vec<&str> {
        let mut res = Vec::new();
        let x = table.get_columns().iter();
        for i in x {
            res.push(i.name().as_str());
        }
        res
    }

    pub fn get_quartiles(table: &DataFrame) -> Vec<(&str, Quartiles)> {
        let cols = Aux::get_columns(table);
        let mut res: Vec<(&str, Quartiles)> = Vec::new();
        for col in cols {
            if let Ok(series) = table.column(col).expect("Column exists").f64() {
                let q = vec![
                    series.min(),
                    series
                        .quantile(0.25, polars::prelude::QuantileMethod::Midpoint)
                        .expect("No Polars error expected."),
                    series.median(),
                    series
                        .quantile(0.75, polars::prelude::QuantileMethod::Midpoint)
                        .expect("No Polars error expected."),
                    series.max(),
                ];
                let q: Vec<f64> = q
                    .iter()
                    .into_iter()
                    .map(|v| v.expect("A column should not be empty"))
                    .collect();
                let q = Quartiles::new(q.as_slice());
                res.push((col, q));
            }
        }
        res
    }

    pub fn draw_box_plot(boxes: Vec<(&str, Quartiles)>) {
        let x_spec: Vec<&str> = boxes.iter().map(|b| b.0).collect();

        // Calculate min and max values inside the quartiles
        let mut y_min = f32::MAX;
        let mut y_max = f32::MIN;
        for b in &boxes {
            let quartiles = b.1.values();
            for v in quartiles {
                if v < y_min {
                    y_min = v
                }
                if v > y_max {
                    y_max = v
                }
            }
        }
        let y_spec = y_min..y_max;

        let boxplots: Vec<Boxplot<_, _>> = boxes
            .iter()
            .map(|b| Boxplot::new_vertical(SegmentValue::CenterOf(&b.0), &(b.1)))
            .collect();

        let figure = evcxr_figure((WIDTH, HEIGHT), |root| {
            root.fill(BACKGROUND)?;
            let mut chart = ChartBuilder::on(&root)
                .margin(MARGIN)
                .x_label_area_size(LABEL_SIZE)
                .y_label_area_size(LABEL_SIZE)
                .build_cartesian_2d(x_spec[..].into_segmented(), y_spec)?;

            chart.configure_mesh().draw()?;

            chart.draw_series(boxplots)?;

            Ok(())
        });

        figure.evcxr_display();
    } */

    pub fn train_and_test(
        model_kind: ModelKind,
        train: &DatasetBase<
            ArrayBase<OwnedRepr<f64>, Dim<[usize; 2]>>,
            ArrayBase<OwnedRepr<bool>, Dim<[usize; 1]>>,
        >,
        test: &DatasetBase<
            ArrayBase<OwnedRepr<f64>, Dim<[usize; 2]>>,
            ArrayBase<OwnedRepr<bool>, Dim<[usize; 1]>>,
        >,
    ) -> Results {
        return match model_kind {
            ModelKind::SVM => {
                let start = std::time::Instant::now();

                let model = Svm::<f64, bool>::params()
                    .nu_weight(0.01)
                    .gaussian_kernel(80.0)
                    .fit(train)
                    .expect("Could not train SVM model");

                let after_training = std::time::Instant::now();

                let predictions = model.predict(test);

                let end = std::time::Instant::now();

                let confusion_matrix = predictions.confusion_matrix(test.targets()).unwrap();

                Results {
                    model: ModelKind::SVM,
                    training_time: after_training.duration_since(start).as_secs_f64(),
                    test_time: end.duration_since(after_training).as_secs_f64(),
                    confusion_matrix,
                }
            }
            ModelKind::LDT => {
                let start = std::time::Instant::now();

                let model = DecisionTree::params()
                    .split_quality(SplitQuality::Gini)
                    .max_depth(Some(100))
                    .min_weight_split(1.0)
                    .fit(&train)
                    .expect("Could not train LDT model");

                let after_training = std::time::Instant::now();

                let predictions = model.predict(test);

                let end = std::time::Instant::now();

                let confusion_matrix = predictions.confusion_matrix(test.targets()).unwrap();

                Results {
                    model: ModelKind::LDT,
                    training_time: after_training.duration_since(start).as_secs_f64(),
                    test_time: end.duration_since(after_training).as_secs_f64(),
                    confusion_matrix,
                }
            }
            ModelKind::GNB => {
                let start = std::time::Instant::now();

                let model = GaussianNbParams::new()
                    .fit(&train)
                    .expect("Could not train GNB model");

                let after_training = std::time::Instant::now();

                let predictions = model.predict(test);

                let end = std::time::Instant::now();

                let confusion_matrix = predictions.confusion_matrix(test.targets()).unwrap();

                Results {
                    model: ModelKind::GNB,
                    training_time: after_training.duration_since(start).as_secs_f64(),
                    test_time: end.duration_since(after_training).as_secs_f64(),
                    confusion_matrix,
                }
            }
        };
    }
}

#[derive(Debug)]
pub enum ModelKind {
    SVM,
    LDT,
    GNB,
}

pub struct Results {
    model: ModelKind,
    training_time: f64,
    test_time: f64,
    confusion_matrix: ConfusionMatrix<bool>,
}

impl Display for Results {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(f, "{:?}:", self.model)?;
        writeln!(f, "Training took {:.2} ms;", self.training_time * 1000.0)?;
        writeln!(f, "Testing took {:.2} ms;", self.test_time * 1000.0)?;
        writeln!(
            f,
            "Accuracy of {:.2} %;",
            self.confusion_matrix.accuracy() * 100.0
        )?;
        writeln!(
            f,
            "Sensitivity of {:.2} %",
            self.confusion_matrix.recall() * 100.0
        )?;
        writeln!(
            f,
            "Precision of {:.2} %",
            self.confusion_matrix.precision() * 100.0
        )?;
        writeln!(f, "F1 scores of {:.2}", self.confusion_matrix.f1_score())?;
        writeln!(f, "Confusion Matrix: \n{:#?}", self.confusion_matrix)?;

        /* Confusion Matrix Layout:
        classes | true | false // <- expected value
        true    | <TP> | <FN>
        false   | <FP> | <TN>
        ^- predicted value
        */
        Ok(())
    }
}
