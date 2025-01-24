document.addEventListener('DOMContentLoaded', async function () {
    let barCtx = document.getElementById('barChart').getContext('2d');
    let doughnutCtx = document.getElementById('doughnutChart').getContext('2d');

    let project_id = document.getElementById('project_id').textContent;
    let statistics = await fetch('/api/projects/' + project_id + '/statistics').then(response => response.json());

    let barChart = new Chart(barCtx, {
        type: 'bar',
        data: {
            labels: statistics.barLabels,
            datasets: [{
                label: 'Tasks Completed by Contributor',
                borderWidth: 1,
                data: statistics.barData
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            scales: {
                y: {
                    beginAtZero: true
                }
            }
        }
    });

    let doughnutChart = new Chart(doughnutCtx, {
        type: 'doughnut',
        data: {
            labels: statistics.doughnutLabels,
            datasets: [{
                label: 'Tasks Completed',
                data: statistics.doughnutData
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false
        }
    });
});