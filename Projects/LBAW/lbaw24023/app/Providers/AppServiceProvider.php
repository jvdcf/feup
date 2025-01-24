<?php

namespace App\Providers;

use Illuminate\Support\Facades\DB;
use Illuminate\Support\ServiceProvider;
use Illuminate\Support\Facades\URL;
use Illuminate\Support\Facades\Auth;
use Illuminate\Support\Facades\View;

use App\Http\Controllers;
use App\Http\Controllers\ProjectController;


class AppServiceProvider extends ServiceProvider
{
    /**
     * Register any application services.
     */
    public function register(): void
    {
        //
    }

    /**
     * Bootstrap any application services.
     */
    public function boot(): void
    {

        View::composer('components.sidebar', function ($view) {
            $projects = Auth::check() ? ProjectController::getUserProjects() : collect();
            $view->with('projects', $projects);  
        });
        View::composer('components.header', function ($view) {
            if (Auth::check() && !Auth::guard('admin')->check()) {
                $notification_count = DB::table('notifications')
                    ->where('user_id', Auth::user()->id)
                    ->count();
                $notification_count += DB::table('task_notifications')
                    ->where('user_id', Auth::user()->id)
                    ->count();
                $view->with('notification_count', $notification_count);
            }
        });
        if(env('FORCE_HTTPS',false)) {
            error_log('configuring https');
            
            $app_url = config("app.url");
            URL::forceRootUrl($app_url);
            $schema = explode(':', $app_url)[0];
            URL::forceScheme($schema);
        }
    }
}
